# Guide utilisateur

Version minimale valide : 1.0.30

## Objectif
Apprendre à utiliser le dashboard une fois le firmware démarré.


## OLED vs LCD : Navigation et affichage
- **OLED (SH1106)** : Monochrome, 128x64, 2 boutons + encodeur rotatif. Navigation : 2 crans = 1 page.
- **LCD (TFT ST7789)** : Couleur, 240x240, 3 boutons + encodeur rotatif. Navigation : 1 cran = 1 page.
- L’interface et la disposition des pages s’adaptent automatiquement à l’afficheur détecté.

## Commandes utilisateur
- **Rotation encodeur** : navigation entre les pages
- **Clic encodeur** : action contextuelle (changement vue prévisions ou action menu)
- **Bouton Back** : page précédente ou fermeture menu
- **Bouton Confirm** : ouverture menu

## Séquence de démarrage
Au boot, l’OLED affiche des étapes de progression :
1. Boot
2. Initialisation capteurs
3. Connexion Wi-Fi
4. Synchronisation horaire (NTP)
5. Prêt

## Pages principales
Le gestionnaire UI fait défiler ces pages :
- Météo
- Prévisions
- Graphe Température
- Graphe Humidité
- Graphe Pression
- Réseau
- Système (LCD : position 7)
- Logs (LCD : position 8)

### Comportement de l’échelle des graphes
- Les libellés min/max des graphes sont calculés à partir de l’historique actuellement affiché à l’écran.
- Les graphes Température et Pression peuvent afficher des décimales pour conserver une différence visible quand les valeurs sont proches.

## Comportement page prévisions
- Défilement automatique toutes les ~5 secondes entre :
  - Aujourd’hui
  - Demain
  - Alertes
- Défilement manuel possible via clic encodeur.

## Logique d’alerte (NeoPixel)
- **Clignotement orange** : alerte météo active
- **Clignotement rouge** : alerte locale température élevée (> 28°C)
- **Vert fixe** : Wi-Fi connecté sans alerte
- **Rouge fixe** : Wi-Fi déconnecté sans alerte météo

## Comportement de sélection des alertes météo
- Le système analyse toutes les alertes météo renvoyées par l’API et conserve la plus critique pour l’affichage.
- À sévérité équivalente, les alertes liées aux crues/inondations sont prioritaires sur les alertes pluie génériques.

## Actions menu
Le menu contient :
- Retour
- Redémarrer
- Effacer les logs
- Effacer l’historique

## Persistance des données
- La dernière page est stockée dans Preferences (NVS).
- L’historique capteurs est stocké dans LittleFS (`/history.dat`).

## Lecture des valeurs météo et interprétation
### Température (Temp)
- Représente la température locale de l’air autour de l’appareil.
- Une hausse rapide en journée peut être normale avec l’ensoleillement ; il faut croiser avec l’humidité et la tendance de pression avant de conclure à un changement météo.
- Pour le ressenti, combiner avec l’humidité : chaud + humide est souvent perçu comme plus chaud que la valeur brute.

### Humidité (Hum)
- L’humidité relative (%) indique la proximité de saturation de l’air.
- Une humidité élevée (souvent > 70 %) peut signaler une ambiance lourde et un risque accru de brume/pluie fine quand la température baisse.
- Une humidité faible (souvent < 35 %) correspond en général à un air plus sec et à des écarts jour/nuit plus marqués.

### Pression (Pres) — indicateur principal
- La tendance de pression est en général plus utile qu’une valeur instantanée.
- Une pression qui baisse sur plusieurs heures indique souvent une dégradation météo (risque vent/pluie en hausse).
- Une pression qui monte sur plusieurs heures indique souvent une amélioration et une météo plus stable.
- Une pression stable suggère peu de changement météo à court terme.

### Repères rapides pression (approximatifs)
- Autour de 1013 hPa : référence neutre.
- Nettement sous cette référence (par exemple < 1005 hPa) : contexte plus perturbé/dépressionnaire probable.
- Nettement au-dessus (par exemple > 1020 hPa) : contexte plus stable/anticyclonique probable.
- Ces seuils sont indicatifs et doivent être interprétés avec l’altitude locale et la tendance.

### Méthode pratique sur ce dashboard
- Lire d’abord la tendance du graphe de pression sur les 1 à 2 dernières heures.
- Confirmer ensuite avec l’évolution de l’humidité (hausse/baisse) et le comportement de la température.
- Vérifier enfin la page Prévisions/Alertes avant de décider.

## Bonnes pratiques d’utilisation
- Laisser le système tourner quelques minutes avant d’interpréter les tendances des graphes.
- Conserver un Wi-Fi stable pour un rafraîchissement fiable des prévisions.
- Consulter la page logs en cas de comportement anormal.
