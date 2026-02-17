# Guide utilisateur

Version minimale valide : 1.0.75

## Objectif
Apprendre à utiliser le dashboard une fois le firmware démarré.


## OLED vs LCD : Navigation et affichage
- **OLED (SH1106)** : Monochrome, 128x64, 2 boutons + encodeur rotatif. Navigation : 2 crans = 1 page.
- **LCD (TFT ST7789)** : Couleur, 240x240 ou 240x320, 3 boutons + encodeur rotatif. Navigation : 1 cran = 1 page.
- L’interface et la disposition des pages s’adaptent automatiquement à l’afficheur détecté.

## Commandes utilisateur
- **Rotation encodeur** : navigation entre les pages
- **Clic encodeur** : ouverture du menu ou validation d'une option
- **Bouton Back** : page précédente ou fermeture menu
- **Bouton Confirm** : action contextuelle (ex: défilement prévisions) ou validation

## Séquence de démarrage
Au boot, l’écran affiche un Splash Screen "morfredus" suivi d'une barre de progression :
1. Splash Screen (Logo Studio & Projet)
2. Initialisation capteurs
3. Connexion Wi-Fi
4. Synchronisation horaire (NTP)
5. Prêt

## Interface Web
Le MeteoHub S3 dispose d'une interface Web complète accessible depuis n'importe quel navigateur sur le même réseau Wi-Fi.

### Accès
- Adresse : `http://meteohub.local` (ou via l'adresse IP affichée sur l'écran au démarrage).

### Fonctionnalités Web
- **Tableau de bord** : Affichage en temps réel de la Température, Humidité et Pression.
- **Graphiques** : Visualisation interactive des 3 courbes sur les dernières 24h (haute résolution).
- **Statistiques** : Page dédiée affichant les Min/Moy/Max pour la période en cours.
- **Historique Long Terme** : Consultation des données archivées sur plusieurs mois/années.
- **Gestionnaire de Fichiers** : Accessible via l'icône dossier 📂 en bas de page. Permet de :
    - Télécharger les fichiers de logs (`system.log`).
    - Télécharger les archives d'historique (`YYYY-MM.dat`).
    - Supprimer ou téléverser des fichiers.

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
- Défilement manuel possible via le bouton **Confirm**.

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
- **Historique Court Terme** : Les dernières 24h (1 point/min) sont en RAM et sauvegardées en continu dans `/history/recent.dat`. Le système recharge ces données au démarrage pour une disponibilité immédiate.
- **Historique Long Terme** : Les données sont archivées mensuellement (moyenne sur 15 min) dans des fichiers `/archive/YYYY-MM.dat` sur la mémoire interne (LittleFS).

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
