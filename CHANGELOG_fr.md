# [1.0.174] – 2026-02-27
- Renforcement de la gestion de la carte SD pour modules ESP32-S3 3.3V avec tentatives de montage SPI adaptatives et reprise améliorée.
- Ajout de vérifications périodiques d’état SD et d’un backoff de reconnexion plus sûr pour réduire les états instables.
- Amélioration des écritures CSV d’historique avec création contrôlée du dossier, flush et nouvelle tentative après remontage.
- Version minimale valide : 1.0.174

# [1.0.173] – 2026-02-26
- Ajout de l'API diagnostic OLED (FPS, durée de rendu, erreurs I2C) exposée dans `/api/system`.
- La page système de l'UI web affiche désormais les diagnostics OLED en temps réel.
- Version minimale valide : 1.0.173
# [1.0.172] – 2026-02-25
- Ajout et liens croisés de la documentation débutant (EN/FR) dans tous les documents utilisateur.
- Tous les guides, FAQ, configuration et index référencent désormais l'onboarding débutant.
- Version minimale valide : 1.0.172

# [1.0.171] – 2026-02-25
- Ajout de la gestion avancée des échelles pour les graphiques température, humidité et pression.
- Trois modes disponibles : fixe, dynamique, mixte (avec élargissement configurable).
- Contrôles interactifs sur l'UI web pour choisir le mode et le pourcentage.
- Aide contextuelle sous le graphique.
- Synchronisation automatique entre config.h et l'UI web.

# [1.0.170] - 2026-02-24
### Corrigé
- Application du même schéma de zone sûre que les graphes aux autres pages OLED.
- Conservation des titres dans la bande haute et déplacement du début de contenu prévisions/logs sous la zone haute réservée SSD1306.
- Ajustement de l’espacement des lignes de logs pour éviter le chevauchement haut sur les SSD1306 à bande jaune.
