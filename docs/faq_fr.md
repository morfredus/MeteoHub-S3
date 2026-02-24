# FAQ

Version minimale valide : 1.0.166

## Quel afficheur est supporté ?
Uniquement OLED (SH1106/SSD1306).

## Quel environnement PlatformIO utiliser ?
`esp32-s3-oled`.

## La carte SD est-elle obligatoire ?
Non, elle est optionnelle pour l’archivage long terme.

## Mon SSD1306 a une bande jaune en haut. L’interface peut-elle chevaucher cette zone ?
Non. Depuis la version 1.0.166, le firmware applique une zone haute réservée sur SSD1306 et décale le rendu pour éviter tout chevauchement.
