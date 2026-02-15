# MeteoHub S3

Version minimale valide : 1.0.22

## Documentation complète
- [Index de la documentation](docs/index_fr.md)

## Présentation
MeteoHub S3 est un projet PlatformIO pour ESP32-S3 qui affiche les données des capteurs locaux, les prévisions météo, les logs et l’état système sur un tableau de bord OLED SH1106.

## Architecture du projet
- `src/modules/` : modules matériels et d’affichage (`encoder`, `neopixel_status`, `sensors`, `sh1106_display`, `pages`)
- `src/managers/` : gestionnaires d’orchestration et d’état (`wifi_manager`, `ui_manager`, `history`, `forecast_manager`)
- `src/utils/` : code utilitaire réutilisable (`logs`, `system_info`)
- `include/` : uniquement les en-têtes de configuration réservés (`board_config.h`, `config.h`, `secrets.h`, `secrets_example.h`, `README`)

## Compilation
- Installer PlatformIO dans VS Code.
- Sélectionner l’environnement `esp32-s3-devkitc-1-n16r8v`.
- Commande de build : `platformio run`
- Commande d’upload : `platformio run --target upload`

## Fonctionnalités
- Acquisition des capteurs (AHT20 + BMP280)
- Gestion Wi-Fi avec logique de reprise
- Synchronisation NTP
- Récupération des prévisions et alertes (OpenWeatherMap)
- Interface OLED par pages avec encodeur et boutons
- Persistance de l’historique dans LittleFS
- LED d’état avec NeoPixel
