
# MeteoHub S3

Version minimale valide : 1.0.30

## Documentation complète
- [Index de la documentation](docs/index_fr.md)

## Présentation
MeteoHub S3 est un projet PlatformIO pour ESP32-S3 qui affiche les données des capteurs locaux, les prévisions météo, les logs et l’état système sur un écran couleur LCD (TFT ST7789) ou un tableau de bord OLED SH1106. Le firmware détecte automatiquement le type d’afficheur et adapte l’interface et la navigation.

## Environnements supportés
- **OLED (SH1106, I2C)** : Monochrome, 128x64, encodeur rotatif + 2 boutons, NeoPixel, capteurs AHT20/BMP280.
- **LCD (TFT ST7789, SPI)** : Couleur, 240x240, encodeur rotatif (HW-040) + 3 boutons, capteurs AHT20/BMP280.

## Architecture du projet
- `src/modules/` : modules matériel/affichage/pages (`encoder`, `neopixel_status`, `sensors`, `sh1106_display`, `st7789_display`, `pages`)
- `src/managers/` : gestionnaires d’orchestration et d’état (`wifi_manager`, `ui_manager`, `history`, `forecast_manager`)
- `src/utils/` : code utilitaire réutilisable (`logs`, `system_info`)
- `include/` : uniquement les en-têtes de configuration réservés (`board_config.h`, `config.h`, `secrets.h`, `secrets_example.h`, `README`)

## Compilation
- Installer PlatformIO dans VS Code.
- Sélectionner l’environnement `esp32-s3-lcd` (TFT) ou `esp32-s3-oled` (OLED) selon le matériel.
- Commande de build : `platformio run`
- Commande d’upload : `platformio run --target upload`

## Fonctionnalités
- Acquisition des capteurs (AHT20 + BMP280)
- Gestion Wi-Fi avec logique de reprise
- Synchronisation NTP
- Récupération des prévisions et alertes (OpenWeatherMap)
- Interface par pages avec encodeur rotatif et boutons (OLED ou LCD)
- Persistance de l’historique dans LittleFS
- LED d’état avec NeoPixel (OLED) ou alerte à l’écran (LCD)

## LCD vs OLED : Différences clés
- **Navigation** : Sur LCD, 1 cran = 1 page ; sur OLED, 2 crans = 1 page.
- **Affichage** : LCD en couleur, haute résolution, supporte des graphismes avancés (axes, courbes, etc.).
- **Câblage** : Voir [Câblage matériel](docs/hardware_wiring_fr.md) pour le détail des broches selon l’environnement.

Pour l’installation, l’utilisation et le dépannage détaillés, consultez la documentation utilisateur dans le dossier `docs/`.
