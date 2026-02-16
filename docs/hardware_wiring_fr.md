# Câblage matériel

Version minimale valide : 1.0.26

## Vue d’ensemble
Cette page explique le mapping GPIO exact utilisé par le firmware.

## Source du mapping des broches
Le mapping de référence est défini dans :
- `include/board_config.h`


## Mapping GPIO actuel

### Environnement OLED (I2C SH1106)
- `OLED_SDA` = GPIO 15
- `OLED_SCL` = GPIO 16
- Encodeur rotatif (EC11) :
	- `ENC_A` = GPIO 4
	- `ENC_B` = GPIO 5
	- `ENC_BTN` = GPIO 6
- Boutons dédiés :
	- `BTN_BACK` = GPIO 7
	- `BTN_CONFIRM` = GPIO 8
- LED de statut :
	- `NEOPIXEL_PIN` = GPIO 48

### Environnement LCD (TFT ST7789)
- Afficheur :
	- `DISPLAY_CS_PIN` = GPIO 13
	- `DISPLAY_MOSI_PIN` = GPIO 11
	- `DISPLAY_SCK_PIN` = GPIO 12
	- `DISPLAY_DC_PIN` = GPIO 10
	- `DISPLAY_RST_PIN` = GPIO 9
	- `DISPLAY_BL_PIN` = GPIO 8
	- `DISPLAY_MISO_PIN` = -1 (non utilisé)
- Encodeur rotatif (HW-040) :
	- `ROTARY_CLK` = GPIO 47
	- `ROTARY_DT` = GPIO 45
	- `ROTARY_SW` = GPIO 40
- Boutons :
	- `BUTTON_BOOT` = GPIO 0
	- `BUTTON_1` = GPIO 38
	- `BUTTON_2` = GPIO 39
- Capteurs I2C :
	- `I2C_SDA` = GPIO 15
	- `I2C_SCL` = GPIO 16

## Notes électriques
- Connecter correctement `3V3` et `GND` de la carte.
- OLED et capteurs doivent partager une masse commune.
- Utiliser des fils I2C courts et stables pour limiter les erreurs de communication.

## Matériel pris en charge
- Environnement OLED : écran OLED SH1106, encodeur EC11, 2 boutons poussoirs, LED NeoPixel, AHT20, BMP280
- Environnement LCD : écran TFT ST7789, encodeur HW-040, 3 boutons poussoirs, AHT20, BMP280

## Checklist de validation
- OLED : écran de boot, encodeur, boutons, NeoPixel, valeurs capteurs
- LCD : écran TFT, encodeur, boutons, valeurs capteurs
