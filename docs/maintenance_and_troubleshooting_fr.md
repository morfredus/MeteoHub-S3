# Maintenance et dépannage

Version minimale valide : 1.0.22

## Objectif
Fournir des étapes de reprise pratiques lorsque le dashboard ne se comporte pas comme prévu.

## Vérifications build et upload
- Build : `platformio run`
- Upload : `platformio run --target upload`
- Nettoyage cache build : `platformio run --target clean`

## Problèmes typiques
### 1) Le Wi-Fi ne se connecte jamais
Vérifier :
- Les valeurs `WIFI_CREDENTIALS` dans `include/secrets.h`
- La disponibilité du routeur
- La qualité du signal près de la carte

### 2) La page prévisions est vide
Vérifier :
- La validité de `OWM_API_KEY`
- La connectivité internet
- Les valeurs latitude/longitude
- Les limites de quota API

### 3) OLED noir
Vérifier :
- Le câblage SDA/SCL (GPIO 15/16)
- L’alimentation et la masse OLED
- Le bon type d’écran SH1106

### 4) Capteurs invalides ou absents
Vérifier :
- Le câblage AHT20/BMP280
- La qualité des pull-up I2C
- La stabilité de l’alimentation capteurs

### 5) Les graphes n’affichent pas de valeurs
Vérifier :
- La synchronisation NTP terminée
- Un temps d’exécution suffisant pour enregistrer des échantillons
- Un historique non effacé récemment

## Actions de maintenance disponibles dans le menu UI
- Redémarrer
- Effacer les logs
- Effacer l’historique

## Workflow de mise à jour sûr
1. Sauvegarder les changements.
2. Compiler en local.
3. Uploader et observer la séquence de boot.
4. Vérifier les pages et les statuts LED.
5. Contrôler la page logs pour détecter les anomalies.
