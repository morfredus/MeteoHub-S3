## Description de l'alerte météo

L’API `/api/alert` retourne désormais le texte complet de l’alerte (en français si disponible). Le dashboard web affiche ce texte pour une clarté et une localisation maximale. Si description est vide, le nom de l’événement est utilisé en secours.

## Affichage des données : Dashboard vs Historique

### Dashboard
Le graphique du tableau de bord affiche uniquement les 2 dernières heures de données capteurs (température, humidité, pression). Cela permet un suivi en temps réel et une visualisation rapide des tendances.

### Page Historique
Le graphique de la page historique affiche les 24 dernières heures de données, utile pour l’analyse à long terme et la comparaison.

Cette distinction garantit que le dashboard reste lisible et rapide, tandis que la page historique offre un contexte détaillé.

## Cartouche alerte météo (Dashboard)

### Qu'est-ce que l'alerte météo ?
Le tableau de bord affiche en permanence une cartouche d'alerte météo en haut de page. Cette cartouche indique l'alerte en cours (s'il y en a une) ou "Aucune alerte". La couleur de la cartouche varie selon la sévérité de l'alerte (jaune pour modérée, rouge pour sévère, vert pour absence d'alerte).

### Fonctionnement
- La cartouche est toujours visible, même en l'absence d'alerte.
- L'information est actualisée en temps réel via l'endpoint `/api/alert`.
- S'il n'y a pas d'alerte, la cartouche affiche "Aucune alerte" et est verte.
- Si une alerte est active, la cartouche affiche l'émetteur, l'événement et la sévérité.

### Exemple (réponse API)
```
GET /api/alert
{
  "active": true,
  "sender": "Météo France",
  "event": "Alerte Orage",
  "severity": "severe"
}
```
En absence d'alerte :
```
GET /api/alert
{
  "active": false
}
```

## Données capteurs en temps réel (API)

L'endpoint `/api/live` retourne les dernières valeurs des capteurs (température, humidité, pression) en temps réel. Ces données sont utilisées pour le dashboard et les statistiques.

### Exemple (réponse API)
```
GET /api/live
{
  "temp": 22.5,
  "hum": 45.0,
  "pres": 1013.2,
  "wifi_rssi": -60,
  "uptime": 123456
}
```

## Conformité stricte au projet

Toutes les routes API sont déclarées uniquement dans la méthode `_setupApi()`. Aucun commentaire interdit ou placeholder n'existe dans le code. La documentation est toujours synchronisée avec le programme et accessible à un débutant.

# Guide utilisateur

Version minimale valide : 1.0.145

## Objectif
Apprendre à utiliser le dashboard une fois le firmware démarré.


## OLED vs LCD : Navigation et affichage
- **OLED (SH1106)** : Monochrome, 128x64, 2 boutons + encodeur rotatif. Navigation : 2 crans = 1 page.
- **LCD (TFT ST7789)** : Couleur, 240x240 ou 240x320, 3 boutons + encodeur rotatif. Navigation : 1 cran = 1 page.
- L’interface et la disposition des pages s’adaptent à l’environnement d’affichage sélectionné (OLED ou LCD).

## Commandes utilisateur
- **Rotation encodeur** : navigation entre les pages
- **Clic encodeur** : ouverture du menu ou validation d'une option
- **Bouton Back** : page précédente ou fermeture menu
- **Bouton Confirm** : action contextuelle (ex: défilement des prévisions, défilement des logs sur OLED) ou validation

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
- **Gestionnaire de Fichiers** : Accessible via l'icône disquette 💾 en bas de page. Permet de naviguer dans la mémoire interne et la carte SD, télécharger ou supprimer des fichiers.
- **Logs Système** : Accessible via l'icône parchemin 📜 en bas de page. Affiche les événements système en temps réel.

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
- Formater la carte SD (si présente)

## Persistance des données
- La dernière page est stockée dans Preferences (NVS).
- **Historique Court Terme** : Les dernières 24h (1 point/min) sont en RAM et sauvegardées en continu dans `/history/recent.dat`. Le système recharge ces données au démarrage pour une disponibilité immédiate.
- **Historique Long Terme** : Si une carte SD est présente, les données sont archivées quotidiennement au format CSV (`/history/YYYY-MM-DD.csv`). C'est le mode de stockage recommandé pour l'archivage.

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
