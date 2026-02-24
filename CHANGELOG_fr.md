# Journal des modifications

Version minimale valide : 1.0.167

## [1.0.167] - 2026-02-24
### Corrigé
- Correction de la compilation des pages OLED via inclusion explicite de `config.h` là où les macros du contrôleur OLED sont utilisées.
- Remplacement de l’expression ternaire `constexpr` basée sur macros par des constantes préprocesseur pour satisfaire l’évaluation stricte à la compilation.
- Conservation du comportement de zone haute sécurisée SSD1306 pour éviter le chevauchement de la bande jaune.
