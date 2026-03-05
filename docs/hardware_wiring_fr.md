# Câblage matériel

Version minimale valide : 1.0.176

Ce projet cible uniquement une configuration OLED.

Références principales :
- `include/board_config.h` pour le mapping des broches
- `include/config.h` pour les réglages runtime d'affichage

Périphériques connectés :
- OLED (I2C)
- Encodeur rotatif + boutons
- AHT20 + BMP280 (I2C)
- NeoPixel
- Carte SD optionnelle (module 3.3V compatible SPI + SDIO)

## Module carte SD (3.3V, compatible SPI + SDIO)

Le gestionnaire SD tente maintenant d'abord le bus SPI (pour les modules D0/SO, CMD/S1 et D3/CS), puis bascule sur SDIO natif si nécessaire.

| Broche module | GPIO | Description               |
|---------------|------|---------------------------|
| CLK           | 12   | Horloge                   |
| CMD / S1      | 11   | SDIO CMD / SPI MOSI (DI)  |
| D0 / SO       | 13   | SDIO DAT0 / SPI MISO (DO) |
| D1            | 14   | SDIO DAT1                 |
| DAT2          | 10   | SDIO DAT2                 |
| D3 / CS       | 9    | SDIO DAT3 / SPI CS        |
| VCC           | 3.3V | Alimentation              |
| GND           | GND  | Masse                     |

> **Note :** Le même mapping GPIO est réutilisé pour les deux bus.
> En mode SPI, D3/CS est la broche de sélection de puce.

### Compatibilité et robustesse

Le pilote tente le montage dans l'ordre suivant :
1. SPI à 1 MHz
2. SPI à 4 MHz
3. SPI à 10 MHz
4. SPI à 20 MHz
5. SDIO 4-bit à 400 kHz
6. SDIO 4-bit à 1 MHz
7. SDIO 4-bit à 4 MHz
8. SDIO 4-bit à 20 MHz
9. SDIO 4-bit à 40 MHz
10. Repli SDIO 1-bit à 400 kHz
11. Repli SDIO 1-bit à 1 MHz
12. Repli SDIO 1-bit à 4 MHz

Cette stratégie maximise la compatibilité avec les modules SD marqués SPI tout en profitant du SDIO quand toutes les lignes de données sont câblées.

## Autres périphériques

| Périphérique   | Broche(s)                     |
|----------------|-------------------------------|
| I2C SDA        | GPIO 15                       |
| I2C SCL        | GPIO 16                       |
| NeoPixel       | GPIO 48                       |
| Encodeur A     | GPIO 4                        |
| Encodeur B     | GPIO 5                        |
| Bouton encodeur| GPIO 6                        |
| Bouton RETOUR  | GPIO 7                        |
| Bouton OK      | GPIO 8                        |
