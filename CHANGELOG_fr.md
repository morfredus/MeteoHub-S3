# [1.2.0] – 2026-02-26
### Modifié
- L’affichage des graphes sur l’OLED est désormais limité aux 2 dernières heures de données.
- L’échelle de l’axe Y sur l’OLED est maintenant dynamique, calculée selon les valeurs min/max de la fenêtre affichée.
- Aucun changement sur l’affichage des graphes de l’interface web.
Version minimale valide : 1.2.0

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
