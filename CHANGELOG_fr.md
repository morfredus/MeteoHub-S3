# [1.0.176] – 2026-03-05
- Ajout de la sélection automatique du backend SD : tentative SPI en premier, puis SDIO (4-bit puis repli 1-bit).
- Mise à jour interne du gestionnaire SD pour exposer un alias de système de fichiers `SD` unifié sur les backends SPI et SDIO.
- Mise à jour de la documentation de câblage SD pour décrire les labels de broches des modules 3.3V SPI/SDIO et la stratégie complète de tentatives.
- Mise à jour des commentaires SD dans `board_config.h` pour refléter l'usage hybride SPI/SDIO.
- Version minimale valide : 1.0.176

# [1.0.175] – 2026-03-04
- Tous les états d'alerte font désormais clignoter la Neopixel (rouge, orange, jaune).
- Seul l'état sans alerte laisse la Neopixel fixe.
- Amélioration des couleurs orange (255,140,0) et jaune vif (255,220,0) pour une meilleure visibilité.
- La luminosité de la Neopixel est maintenant configurable dans config.h (défaut : 15%).
- Version minimale valide : 1.0.175

# [1.0.173] – 2026-03-04
- Remplacement du module SD SPI par un module SD natif SDMMC 4-bit 3.3V (broches : CLK, CMD, DAT0, DAT1, DAT2, DAT3/CD).
- Mise à jour de `board_config.h` : remplacement des broches SD_CS/SCK/MOSI/MISO par SD_CLK/CMD/DAT0/DAT1/DAT2/DAT3 (GPIO 9, 10, 11, 12, 13, 14).
- Réécriture complète de `sd_manager` pour utiliser le pilote SDMMC natif ESP32 (`SD_MMC.h`) à la place de SPI (`SD.h`).
- Ajout d'un repli robuste 4-bit → 1-bit avec tentatives multi-fréquences pour les modules de qualité moyenne.
- Utilisation de `esp_vfs_fat_sdcard_format` pour le formatage explicite déclenché par l'utilisateur.
- Mise à jour de `system_info.cpp` pour utiliser l'include `SD_MMC.h`.
- Version minimale valide : 1.0.173

# [1.0.172] – 2026-02-25
- Ajout et liens croisés de la documentation débutant (EN/FR) dans tous les documents utilisateur.
- Tous les guides, FAQ, configuration et index référencent désormais l'onboarding débutant.
- Version minimale valide : 1.0.172

# [1.0.171] – 2026-02-25
- Ajout de la gestion avancée des échelles pour les graphiques température, humidité et pression.
- Trois modes disponibles : fixe, dynamique, mixte (avec élargissement configurable).
- Contrôles interactifs sur l'UI web pour choisir le mode et le pourcentage.
- Aide contextuelle sous le graphique.
- Synchronisation automatique entre config.h et l'UI web.

# [1.0.170] - 2026-02-24
### Corrigé
- Application du même schéma de zone sûre que les graphes aux autres pages OLED.
- Conservation des titres dans la bande haute et déplacement du début de contenu prévisions/logs sous la zone haute réservée SSD1306.
- Ajustement de l’espacement des lignes de logs pour éviter le chevauchement haut sur les SSD1306 à bande jaune.
