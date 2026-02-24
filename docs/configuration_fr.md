# Configuration

Version minimale valide : 1.0.166

## Fichiers modifiables
- `include/secrets.h` (identifiants, clé API, coordonnées)
- `include/config.h` (rafraîchissement, contraste OLED, nom mdns)

## Fichier réservé
- `include/board_config.h` (mapping matériel)

## Métadonnées
Le nom/la version du projet sont injectés depuis `platformio.ini` via build flags.
