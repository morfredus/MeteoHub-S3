# Câblage matériel

Version minimale valide : 1.0.180

Ce projet cible uniquement une configuration OLED.

Références principales :
- `include/board_config.h` pour le mapping des broches
- `include/config.h` pour les réglages runtime d’affichage

Périphériques connectés :
- OLED (I2C)
- Encodeur rotatif + boutons
- AHT20 + BMP280 (I2C)
- NeoPixel
- Module microSD Sniffer optionnel (SD_MMC 1-bit)

## Câblage microSD Sniffer (ESP32-S3)
- VCC -> 3V3
- GND -> GND
- CLK -> GPIO12
- CMD -> GPIO11
- DAT0 -> GPIO13
- CD -> GPIO10 (détection carte optionnelle)
- DAT1/DAT2 : non utilisés en mode 1-bit
