# Câblage matériel

Version minimale valide : 1.0.119

## Vue d’ensemble
Cette page explique le mapping GPIO exact utilisé par le firmware.


## Environnements OLED/LCD
Le firmware prend en charge à la fois l’OLED SH1106 et le LCD ST7789. Le mapping des broches et la disposition des boutons diffèrent selon l’environnement. Voir ci-dessous pour le détail. Le mapping de référence est toujours défini dans :
- `include/board_config.h`


## Mapping GPIO actuel

### Environnement OLED (I2C SH1106)
- `I2C_SDA_PIN` = GPIO 15
- `I2C_SCL_PIN` = GPIO 16
- Encodeur rotatif (EC11) :
	- `ENCODER_A_PIN` = GPIO 4
	- `ENCODER_B_PIN` = GPIO 5
	- `ENCODER_BTN_PIN` = GPIO 6
- Boutons dédiés :
	- `BUTTON_BACK_PIN` = GPIO 7
	- `BUTTON_CONFIRM_PIN` = GPIO 8
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
	- `ENCODER_A_PIN` = GPIO 47
	- `ENCODER_B_PIN` = GPIO 45
	- `ENCODER_BTN_PIN` = GPIO 40
- Boutons :
	- `BUTTON_BOOT_PIN` = GPIO 0
	- `BUTTON_BACK_PIN` = GPIO 38
	- `BUTTON_CONFIRM_PIN` = GPIO 39
- Capteurs I2C :
	- `I2C_SDA_PIN` = GPIO 15
	- `I2C_SCL_PIN` = GPIO 16

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
