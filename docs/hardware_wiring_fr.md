# Câblage matériel

Version minimale valide : 1.0.173

Ce projet cible uniquement une configuration OLED.

Références principales :
- `include/board_config.h` pour le mapping des broches
- `include/config.h` pour les réglages runtime d'affichage

Périphériques connectés :
- OLED (I2C)
- Encodeur rotatif + boutons
- AHT20 + BMP280 (I2C)
- NeoPixel
- Carte SD optionnelle (interface native SDMMC 4-bit, 3.3V)

## Module carte SD (SDMMC natif 4-bit, 3.3V)

La carte SD utilise l'interface SDMMC native de l'ESP32-S3 (et non le bus SPI).
Cela offre de meilleures performances et une meilleure fiabilité que le mode SPI.

| Broche module | GPIO | Description               |
|---------------|------|---------------------------|
| CLK           | 12   | Horloge                   |
| CMD           | 11   | Commande                  |
| DAT0          | 13   | Ligne de données 0        |
| DAT1          | 14   | Ligne de données 1        |
| DAT2          | 10   | Ligne de données 2        |
| CD / DAT3     | 9    | Détection carte / données 3 |
| VCC           | 3.3V | Alimentation              |
| GND           | GND  | Masse                     |

> **Note :** La broche CD du module correspond à DAT3 dans le protocole SDMMC 4-bit.
> Elle doit être connectée au GPIO 9 comme indiqué ci-dessus.

### Compatibilité et robustesse

Le pilote tente le montage dans l'ordre suivant :
1. Mode 4-bit à 400 kHz (le plus conservateur)
2. Mode 4-bit à 1 MHz
3. Mode 4-bit à 4 MHz
4. Mode 4-bit à 20 MHz
5. Mode 4-bit à 40 MHz
6. Repli 1-bit à 400 kHz (pour les câblages bruyants ou modules de qualité moyenne)
7. Repli 1-bit à 1 MHz
8. Repli 1-bit à 4 MHz

Cette stratégie assure la compatibilité avec des modules SD de qualité moyenne et des cartes breakout à courtes pistes.

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
