# MeteoHub S3

Version minimale valide : 1.0.156

## Documentation complète
- [Index de la documentation](docs/index_fr.md)

## Présentation
MeteoHub S3 est un projet PlatformIO pour ESP32-S3 qui affiche les données capteurs locales, les prévisions météo, les logs et l’état système sur un tableau de bord OLED (SH1106/SSD1306 via U8g2).

## Matériel requis
- Écran OLED (SH1106 ou SSD1306, I2C)
- Module rotatif HW-040 (encodeur + clic)
- Boutons Back et Confirm
- Capteurs AHT20 + BMP280
- Carte SD optionnelle pour l’historique long terme

## Compilation
- Installer PlatformIO dans VS Code.
- Sélectionner l’environnement `esp32-s3-oled`.
- Build : `platformio run`
- Upload : `platformio run --target upload`

## Fonctionnalités clés
- Pages météo, prévisions, graphes d’historique, réseau/système et logs sur OLED.
- Support de mise à jour OTA (ArduinoOTA + page OTA web).
- Formatage SD et gestion d’historique depuis le menu appareil.
- Dashboard web avec données live, graphes, logs et gestionnaire de fichiers.

## Notes
- Le rendu OLED est assuré par U8g2 et prend en charge SH1106/SSD1306.
- Pour l’installation détaillée, l’utilisation et le dépannage, consulter le dossier `docs/`.
