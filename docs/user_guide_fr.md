# Guide utilisateur

Version minimale valide : 1.0.22

## Objectif
Apprendre à utiliser le dashboard une fois le firmware démarré.

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
- Logs
- Système

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

## Actions menu
Le menu contient :
- Retour
- Redémarrer
- Effacer les logs
- Effacer l’historique

## Persistance des données
- La dernière page est stockée dans Preferences (NVS).
- L’historique capteurs est stocké dans LittleFS (`/history.dat`).

## Bonnes pratiques d’utilisation
- Laisser le système tourner quelques minutes avant d’interpréter les tendances des graphes.
- Conserver un Wi-Fi stable pour un rafraîchissement fiable des prévisions.
- Consulter la page logs en cas de comportement anormal.
