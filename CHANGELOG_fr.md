# Journal des modifications

Version minimale valide : 1.0.166

## [1.0.166] - 2026-02-24
### Corrigé
- Ajout d’un décalage de rendu haut sécurisé pour SSD1306 afin d’éviter le chevauchement avec les variantes à bande jaune.
- Décalage de toutes les pages OLED (en-tête, lignes, graphes, prévisions, logs, écrans de boot) pour respecter la zone réservée en haut des écrans SSD1306.

### Notes
- La cible OLED unique reste inchangée.
