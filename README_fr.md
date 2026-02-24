# MeteoHub S3

> **Version minimale valide : 1.0.169**

## Documentation complète
- [Index de la documentation](docs/index_fr.md)

## Présentation
MeteoHub S3 est un projet PlatformIO pour ESP32-S3 centré sur un tableau de bord OLED (SH1106/SSD1306 via U8g2). Il affiche les données capteurs locales, les prévisions météo, les logs et l’état système.

## Matériel nécessaire
- Écran OLED (SH1106 ou SSD1306, I2C)
- Module encodeur HW-040 (encodeur + bouton central)
- Boutons Back et Confirm
- Capteurs AHT20 et BMP280
- Carte SD optionnelle pour l’archivage long terme

## Compilation
- Installer PlatformIO dans VS Code
- Sélectionner l’environnement : `esp32-s3-oled`
- Build : `platformio run`
- Upload : `platformio run --target upload`

## Fonctionnalités principales
- Affichage temps réel des capteurs
- Pages de prévisions météo
- Pages de logs locales
- Historique persistant (LittleFS + archivage SD optionnel)
- Mise à jour OTA via l’interface web
- Rendu UTF-8 sur OLED

Pour l’installation, l’utilisation et le dépannage, consultez `docs/`.
