# [1.0.175] – 2026-03-03
- Ajout d'un durcissement spécifique EC11 (mode HalfQuad + filtre encodeur renforcé) pour réduire les tempêtes d'interruptions observées sur les modules SH1106 tout-en-un.
- Ajout d'un anti-rebond / limitation logicielle des événements de rotation pour éviter la surcharge OLED/UI et les freezes pendant la manipulation du bouton rotatif.
- Version minimale valide : 1.0.175

# [1.0.174] – 2026-03-02
- Correction des freezes/artefacts restants lors de la rotation de l'encodeur en limitant la cadence de redraw OLED.
- Réduction de la contention I2C via mise en cache courte des lectures capteurs pour éviter des sondages répétés pendant les changements de page rapides.
- Version minimale valide : 1.0.174

# [1.0.173] – 2026-03-02
- Correction des freezes intermittents en réduisant les écritures NeoPixel bloquantes (mise en cache de la couleur) et en ajoutant une cession coopérative dans la boucle principale.
- Réduction du risque de fragmentation mémoire en remplaçant le vector de logs avec erase/push par un buffer circulaire fixe.
- Renforcement de l'initialisation OLED en forçant l'orientation normale (pas de miroir/inversion) et une horloge I2C stable.
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
