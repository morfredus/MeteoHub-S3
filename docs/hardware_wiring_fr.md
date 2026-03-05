# Câblage matériel

Version minimale valide : 1.0.182

Ce projet cible uniquement une configuration OLED.

Références principales :
- `include/board_config.h` pour le mapping des broches
- `include/config.h` pour les réglages runtime d’affichage

Périphériques connectés :
- OLED (I2C)
- Encodeur rotatif + boutons
- AHT20 + BMP280 (I2C)
- NeoPixel
- Module SD 3.3V optionnel (SPI principal, repli SDIO 1-bit)

## Câblage module SD 3.3V (ESP32-S3)
- VCC -> 3V3
- GND -> GND
- CLK -> GPIO12
- D0/SO -> GPIO13
- CMD/SI -> GPIO11
- D3/CS -> GPIO10
- D1 / DAT2 : non utilisés
- DET4 : optionnel (non utilisé dans le code par défaut)

Stratégie runtime :
- Mode principal : SPI
- Mode de repli : SD_MMC 1-bit