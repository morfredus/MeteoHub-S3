# Câblage matériel

Version minimale valide : 1.0.177

Ce projet cible uniquement une configuration OLED.

Références principales :
- `include/board_config.h` pour le mapping des broches
- `include/config.h` pour les réglages runtime d’affichage

Périphériques connectés :
- OLED (I2C)
- Encodeur rotatif + boutons
- AHT20 + BMP280 (I2C)
- NeoPixel
- Carte SD optionnelle (SPI)

Environnements PlatformIO :
1. `esp32-s3-oled` pour ESP32-S3 DevKitC-1
2. `esp32-dev-oled` pour ESP32 Dev Module


## Fiabilité de la carte SD (modules ESP32-S3 3.3V)
1. Garder les fils SPI courts avec une masse commune stable.
2. Utiliser un module SD compatible 3,3V (pas un adaptateur 5V uniquement).
3. Le firmware effectue désormais des tentatives de montage SPI adaptatives (8MHz à 400kHz).
4. Des vérifications d’état et un remontage automatique protègent les sessions longues.
5. Les écritures d’historique sont retentées proprement après remontage pour limiter les pertes.
