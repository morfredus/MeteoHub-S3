# Câblage matériel

Version minimale valide : 1.0.20

## Vue d’ensemble
Cette page explique le mapping GPIO exact utilisé par le firmware.

## Source du mapping des broches
Le mapping de référence est défini dans :
- `include/board_config.h`

## Mapping GPIO actuel
### OLED (I2C SH1106)
- `OLED_SDA` = GPIO 15
- `OLED_SCL` = GPIO 16

### Encodeur rotatif (EC11)
- `ENC_A` = GPIO 4
- `ENC_B` = GPIO 5
- `ENC_BTN` = GPIO 6

### Boutons dédiés
- `BTN_BACK` = GPIO 7
- `BTN_CONFIRM` = GPIO 8

### LED de statut
- `NEOPIXEL_PIN` = GPIO 48

## Notes électriques
- Connecter correctement `3V3` et `GND` de la carte.
- OLED et capteurs doivent partager une masse commune.
- Utiliser des fils I2C courts et stables pour limiter les erreurs de communication.

## Capteurs pris en charge dans ce projet
- AHT20 (température/humidité)
- BMP280 (pression + température de secours)

## Checklist de validation
- L’OLED affiche l’écran de boot
- L’encodeur tourne et clique
- Les boutons Back/Confirm réagissent
- Le NeoPixel affiche les couleurs de statut
- Les valeurs capteurs apparaissent sur la page météo
