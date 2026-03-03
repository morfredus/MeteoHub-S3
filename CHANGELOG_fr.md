# [1.0.180] – 2026-03-03
- Migration du support SD de l'ancien module SPI vers le module microSD Sniffer (mode SD_MMC 1-bit).
- Ajout du mapping SD_MMC (CLK/CMD/DAT0/CD) et migration du gestionnaire SD, de l'historique et des infos système vers les APIs SD_MMC.
- Version minimale valide : 1.0.180

# [1.0.179] – 2026-03-03
- Correction robuste de la compilation encodeur en remplaçant la constante membre de debounce rotation par une méthode constexpr unique (plus de chemin de double déclaration).
- Comportement conservé : un cran = un changement de page/ligne, avec debounce par contrôleur (SH1106 : 8ms, SSD1306 : 4ms).
- Version minimale valide : 1.0.179

# [1.0.178] – 2026-03-03
- Correction de l'erreur de compilation encodeur en regroupant la constante de debounce rotation en une seule déclaration dans `encoder.h`.
- Conservation des valeurs de debounce par matériel (SH1106/EC11 : 8ms, SSD1306/KY-040 : 4ms) avec un cran par changement de page/ligne.
- Version minimale valide : 1.0.178

# [1.0.177] – 2026-03-03
- Correction du mapping des crans encodeur selon le profil matériel : SH1106+EC11 et SSD1306+KY-040 passent désormais d'une page/ligne par cran.
- La sélection du profil encodeur suit maintenant la configuration du contrôleur OLED (SH1106 vs SSD1306).
- Version minimale valide : 1.0.177

# [1.0.176] – 2026-03-03
- Ajustement de l'échelle des pas encodeur selon le matériel : EC11 conserve le comportement actuel, KY-040 passe à un seul cran pour changer une page/ligne.
- Version minimale valide : 1.0.176

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
