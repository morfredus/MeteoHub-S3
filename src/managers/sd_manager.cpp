#include "sd_manager.h"
#include "../utils/logs.h"
#include "board_config.h"
#include <algorithm>
#include <driver/gpio.h>
#include "ff.h" // For f_mkfs
 
// Déclarations des fonctions internes de la librairie SD (sd_diskio.cpp)
// Nécessaire pour initialiser la carte sans la monter (ce qui échoue si non formatée)
uint8_t sdcard_init(uint8_t cs, SPIClass * spi, int hz);
bool sdcard_uninit(uint8_t pdrv);
 
namespace {
 
 // =================================================================
 // 1. Constantes et Configuration
 // =================================================================
 
 // Fréquences SPI à tester pour le montage (du plus rapide au plus lent)
 constexpr int SD_INIT_FREQUENCIES[] = {8000000, 4000000, 1000000, 400000};
 // Fréquences SPI à tester pour le formatage (plus conservateur)
 constexpr int SD_FORMAT_FREQUENCIES[] = {4000000, 1000000, 400000};
 
 // Délais pour la gestion de l'alimentation du module SD
 constexpr int SD_POWER_OFF_DELAY_MS = 50;
 constexpr int SD_POWER_ON_STABILIZE_MS = 200;
 
 // Paramètres pour la logique de reconnexion automatique
 constexpr unsigned long SD_RECONNECT_COOLDOWN_DEFAULT_MS = 15000;
 constexpr unsigned long SD_RECONNECT_COOLDOWN_MAX_MS = 120000; // 2 minutes max
 
 // Pin pour contrôler l'alimentation du module SD (si -1, non utilisé)
 #ifndef SD_OFF_PIN
 #define SD_OFF_PIN -1
 #endif
 
 // =================================================================
 // 2. Fonctions d'aide (privées)
 // =================================================================
 
 /**
  * @brief Vérifie si le contrôle de l'alimentation du module SD est configuré.
  */
 bool hasSdOffControl() {
     return SD_OFF_PIN >= 0;
 }
 
 /**
  * @brief Active ou désactive l'alimentation du module SD.
  * @param power_on true pour allumer, false pour éteindre.
  */
 void setSdModulePower(bool power_on) {
     if (!hasSdOffControl()) return;
     pinMode(SD_OFF_PIN, OUTPUT);
     // La logique peut varier selon le matériel (ex: uPesy HIGH=ON, LOW=OFF)
     digitalWrite(SD_OFF_PIN, power_on ? HIGH : LOW);
 }
 
 /**
  * @brief Effectue un cycle d'alimentation (OFF/ON) sur le module SD si supporté.
  * C'est une méthode très efficace pour réinitialiser une carte SD bloquée.
  */
 void powerCycleSdModule() {
     if (!hasSdOffControl()) return;
     
     // Désactiver le bus SPI et mettre les pins en flottant/LOW pour éviter
     // l'alimentation parasite via les diodes de protection du module SD.
     SPI.end();
     pinMode(SD_SCK_PIN, INPUT);
     pinMode(SD_MOSI_PIN, INPUT);
     pinMode(SD_CS_PIN, INPUT);
     
     setSdModulePower(false);
     delay(SD_POWER_OFF_DELAY_MS);
     setSdModulePower(true);
     delay(SD_POWER_ON_STABILIZE_MS);
 }
 
 /**
  * @brief Configure les pins GPIO pour le bus SPI de la carte SD.
  * Augmente la force du signal (drive strength) pour améliorer la stabilité
  * sur des fils longs ou dans un environnement bruité.
  */
 void initializeSpiBus() {
     pinMode(SD_SCK_PIN, OUTPUT);
     pinMode(SD_MOSI_PIN, OUTPUT);
     pinMode(SD_CS_PIN, OUTPUT);
     pinMode(SD_MISO_PIN, INPUT_PULLUP);
     digitalWrite(SD_CS_PIN, HIGH);
 
     gpio_set_drive_capability(static_cast<gpio_num_t>(SD_SCK_PIN), GPIO_DRIVE_CAP_3);
     gpio_set_drive_capability(static_cast<gpio_num_t>(SD_MOSI_PIN), GPIO_DRIVE_CAP_3);
     gpio_set_drive_capability(static_cast<gpio_num_t>(SD_CS_PIN), GPIO_DRIVE_CAP_3);
 
     SPI.begin(SD_SCK_PIN, SD_MISO_PIN, SD_MOSI_PIN, -1); // CS géré manuellement
 }
 
 /**
  * @brief Tente de monter la carte SD à une fréquence SPI donnée.
  */
 bool tryMountAtFrequency(int frequency_hz) {
     LOG_INFO("SD mount attempt at " + std::to_string(frequency_hz / 1000) + " kHz...");
     if (SD.begin(SD_CS_PIN, SPI, frequency_hz)) {
         if (SD.cardType() != CARD_NONE) {
             LOG_INFO("SD mount OK at " + std::to_string(frequency_hz / 1000) + " kHz");
             return true;
         }
         LOG_WARNING("SD mount reported OK but card type is NONE. Un-mounting.");
         SD.end();
     } else {
         LOG_WARNING("SD mount failed at " + std::to_string(frequency_hz / 1000) + " kHz");
     }
     return false;
 }
 
 /**
  * @brief Tente un formatage bas niveau à une fréquence SPI donnée.
  * Utilise f_mkfs de la librairie FatFS sous-jacente.
  */
 bool tryLowLevelFormat(int frequency_hz) {
     uint8_t pdrv = 0xFF;
 
     // Envoi de >74 cycles d'horloge avec CS=HIGH pour mettre la carte en mode SPI natif.
     // C'est une étape cruciale pour la compatibilité avec certaines cartes.
     digitalWrite(SD_CS_PIN, HIGH);
     for (int i = 0; i < 16; i++) { // 16 * 8 = 128 cycles
         SPI.transfer(0xFF);
     }
 
     // 1. Initialisation bas niveau de la carte
     // Le bus SPI est déjà initialisé par la fonction appelante (format)
     pdrv = sdcard_init(SD_CS_PIN, &SPI, frequency_hz);
     if (pdrv == 0xFF) {
         LOG_WARNING("SD format: low-level init failed at " + std::to_string(frequency_hz / 1000) + " kHz");
         return false;
     }
     LOG_INFO("SD format: low-level init OK (pdrv=" + std::to_string(pdrv) + ") at " + std::to_string(frequency_hz / 1000) + " kHz");
 
     // 2. Formatage avec f_mkfs
     char drv[4];
     snprintf(drv, sizeof(drv), "%d:", pdrv);
     uint8_t* work_buffer = static_cast<uint8_t*>(malloc(FF_MAX_SS));
     if (!work_buffer) {
         LOG_ERROR("SD format: failed to allocate work buffer");
         sdcard_uninit(pdrv); // Cleanup
         return false;
     }
 
     FRESULT res = f_mkfs(drv, FM_ANY, 0, work_buffer, FF_MAX_SS);
     free(work_buffer);
 
     // 3. Libération des ressources de la carte (mais pas du bus SPI)
     sdcard_uninit(pdrv);
 
     if (res != FR_OK) {
         LOG_WARNING("SD format failed with f_mkfs error code: " + std::to_string(res));
         return false;
     }
 
     return true;
 }
 
 /**
  * @brief Affiche le mapping des pins SD pour le diagnostic.
  */
 void logSdPinMapping() {
     LOG_INFO(
         "SD pin mapping: CS=" + std::to_string(SD_CS_PIN) +
         ", SCK=" + std::to_string(SD_SCK_PIN) +
         ", MISO=" + std::to_string(SD_MISO_PIN) +
         ", MOSI=" + std::to_string(SD_MOSI_PIN)
     );
     if (hasSdOffControl()) {
         LOG_INFO("SD power control enabled on GPIO " + std::to_string(SD_OFF_PIN));
     }
 }
 
 } // namespace
 
 // =================================================================
 // 3. Implémentation de la classe SdManager
 // =================================================================
 
 bool SdManager::begin() {
     LOG_INFO("Initializing SD Card...");
     logSdPinMapping();
 
     // S'assurer que le module est alimenté au démarrage
     setSdModulePower(true);
     delay(SD_POWER_ON_STABILIZE_MS);
 
     if (mountWithRetries()) {
         LOG_INFO("SD Card OK. Size: " + std::to_string(SD.cardSize() / (1024 * 1024)) + "MB");
         _last_reconnect_attempt_ms = millis();
         _consecutive_reconnect_failures = 0;
         _reconnect_cooldown_ms = SD_RECONNECT_COOLDOWN_DEFAULT_MS;
         return true;
     }
 
     LOG_ERROR("SD mount failed on all retries. Check wiring/card or run format.");
     return false;
 }
 
 bool SdManager::mountWithRetries() {
     SD.end();
     _available = false;
 
     powerCycleSdModule();
     initializeSpiBus();
 
     for (int frequency_hz : SD_INIT_FREQUENCIES) {
         // Envoi de cycles d'horloge "à vide" pour réveiller la carte (mode SPI)
         // Essentiel après un cycle d'alimentation.
         digitalWrite(SD_CS_PIN, HIGH);
         for (int i = 0; i < 16; i++) SPI.transfer(0xFF);
 
         if (tryMountAtFrequency(frequency_hz)) {
             if (!SD.exists("/history")) {
                 LOG_INFO("Creating /history directory on SD card.");
                 SD.mkdir("/history");
             }
             _available = true;
             return true;
         }
         SD.end(); // Nettoyage du driver en cas d'échec avant la prochaine tentative
         delay(100); // Court délai entre les essais
     }
 
     return false;
 }
 
 bool SdManager::isAvailable() {
     if (!_available) {
         return false;
     }
     // Test rapide pour voir si la carte est toujours présente
     if (SD.cardType() == CARD_NONE) {
         LOG_WARNING("SD card became unavailable (cardType is NONE).");
         _available = false;
         SD.end();
         return false;
     }
     return true;
 }
 
 bool SdManager::ensureMounted() {
     if (isAvailable()) {
         return true;
     }
 
     unsigned long now = millis();
     if (now - _last_reconnect_attempt_ms < _reconnect_cooldown_ms) {
         return false; // Respecter le cooldown pour ne pas surcharger le bus
     }
 
     _last_reconnect_attempt_ms = now;
     LOG_INFO("Attempting to reconnect SD card (cooldown=" + std::to_string(_reconnect_cooldown_ms) + "ms)...");
 
     if (mountWithRetries()) {
         LOG_INFO("SD card reconnected successfully.");
         _consecutive_reconnect_failures = 0;
         _reconnect_cooldown_ms = SD_RECONNECT_COOLDOWN_DEFAULT_MS;
         return true;
     }
 
     // Augmenter le cooldown en cas d'échecs consécutifs (backoff exponentiel simple)
     _consecutive_reconnect_failures++;
     _reconnect_cooldown_ms = std::min(
         SD_RECONNECT_COOLDOWN_MAX_MS,
         SD_RECONNECT_COOLDOWN_DEFAULT_MS * (1 + _consecutive_reconnect_failures)
     );
 
     LOG_WARNING("SD reconnect failed (failure #" + std::to_string(_consecutive_reconnect_failures) + "). Next attempt in " + std::to_string(_reconnect_cooldown_ms) + "ms.");
     return false;
 }
 
 bool SdManager::format() {
     LOG_WARNING("Formatting SD Card... This will erase all data!");
 
     if (_available) {
         SD.end();
         _available = false;
     }
 
     powerCycleSdModule();
     initializeSpiBus(); // Configure pins, drive strength, and call SPI.begin()
 
     bool format_success = false;
     for (int i = 0; i < (sizeof(SD_FORMAT_FREQUENCIES) / sizeof(int)); ++i) {
         int frequency_hz = SD_FORMAT_FREQUENCIES[i];
         LOG_INFO("SD format attempt #" + std::to_string(i + 1) + " at " + std::to_string(frequency_hz / 1000) + " kHz...");
         if (tryLowLevelFormat(frequency_hz)) {
             LOG_INFO("SD format successful!");
             format_success = true;
             break;
         }
         delay(200);
     }
 
     SPI.end(); // Clean up SPI before calling begin()
 
     if (!format_success) {
         LOG_ERROR("SD format failed after all retries.");
         return false;
     }
 
     // Après un formatage réussi, on tente de remonter la carte immédiatement.
     LOG_INFO("Remounting card after format...");
     if (!begin()) {
         LOG_ERROR("SD card was formatted but failed to remount.");
         return false;
     }
 
     return true;
 }
