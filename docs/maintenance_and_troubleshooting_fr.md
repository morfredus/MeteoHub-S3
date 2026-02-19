# Maintenance et dépannage

Version minimale valide : 1.0.115

## Objectif
Fournir des étapes de reprise pratiques lorsque le dashboard ne se comporte pas comme prévu.

## Vérifications build et upload
- Build : `platformio run`
- Upload : `platformio run --target upload`
- Nettoyage cache build : `platformio run --target clean`

## Environnements OLED/LCD : auto-détection et navigation
Le firmware détecte et s’adapte automatiquement à un écran OLED SH1106 (128x64, monochrome) ou un LCD TFT ST7789 (240x240/320, couleur) au démarrage. La navigation et le mapping des boutons diffèrent selon l’environnement (voir guide utilisateur et câblage matériel).

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
Ce comportement n'est plus normal. Le système recharge l'historique au démarrage. Si les graphes restent vides, cela peut indiquer un problème avec le système de fichiers LittleFS. Essayez d'effacer l'historique via le menu système pour forcer une réinitialisation.

### 6) L'interface Web est inaccessible
Vérifier :
- Que vous êtes sur le même réseau Wi-Fi.
- Essayez l'adresse IP directe (ex: `192.168.1.x`) au lieu de `meteohub.local` (le mDNS peut être capricieux sur certains réseaux/OS).
- Vérifiez que la partition LittleFS est bien montée (voir logs au démarrage).

## Actions de maintenance disponibles dans le menu UI
- Redémarrer
- Effacer les logs
- Effacer l’historique
- Formater la carte SD (si présente)

### 7) Redémarrages en boucle ou blocage au démarrage (Watchdog Triggered)
**Symptômes** : L'appareil redémarre en boucle, les logs affichent `Task watchdog got triggered` et/ou des erreurs `vfs_api.cpp:105] open(): ... does not exist, no permits for creation`. L'interface web ne répond plus.

**Cause** : Le système de fichiers (LittleFS) est très probablement corrompu. Cela arrive le plus souvent après une coupure de courant ou une déconnexion USB brutale pendant que l'appareil écrivait des données (historique, logs).

**Récupération facile (formatage d'urgence) :**

> **Astuce pour débutants :** Si votre MeteoHub S3 est bloqué ou redémarre en boucle, vous pouvez forcer un formatage d'urgence de la mémoire interne (LittleFS) sans ordinateur, simplement en maintenant le bouton BOOT au démarrage.

**Procédure étape par étape :**
1. Débranchez l'appareil de son alimentation USB.
2. Maintenez le bouton **BOOT** enfoncé (généralement étiqueté "BOOT" ou connecté au GPIO 0).
3. Tout en maintenant BOOT, rebranchez l'alimentation USB.
4. Continuez de maintenir BOOT pendant environ 3 secondes. L'écran affichera un message de maintenance et indiquera que le formatage va commencer.
5. Relâchez le bouton BOOT lorsque le message de formatage apparaît. L'appareil effacera la mémoire interne (toutes les données d'historique seront perdues) puis redémarrera automatiquement.

Ce mode de récupération est conçu pour être accessible à tout utilisateur, même débutant, et ne nécessite aucun logiciel ou outil externe.

### 8) Erreurs I2C (`i2cRead returned Error -1`) et redémarrages inattendus
**Symptômes** : Les logs affichent des erreurs `i2cRead returned Error -1` et/ou `Bus already started in Master Mode`. L'appareil peut redémarrer de manière inattendue, parfois avec un message `Reason: 8 - ASSOC_LEAVE` dans les logs Wi-Fi.

**Causes et Solutions** :
- **`Bus already started...`** : C'est un avertissement normal et sans danger. Il apparaît car plusieurs composants tentent d'initialiser le bus I2C. Vous pouvez l'ignorer.
- **`i2cRead returned Error -1`** : C'est une erreur matérielle. Cela signifie qu'un capteur (AHT20 ou BMP280) ne répond pas.
    - **Vérifiez le câblage** des broches SDA, SCL, VCC et GND de vos capteurs.
    - **Vérifiez l'alimentation** : Une alimentation USB faible ou un câble de mauvaise qualité peuvent causer des baisses de tension qui provoquent ces erreurs. Essayez un autre port USB, un autre câble, ou une alimentation murale dédiée.
- **Redémarrage inattendu (`ASSOC_LEAVE`)** : Le redémarrage est souvent une conséquence de l'instabilité causée par le problème matériel ci-dessus. Une alimentation instable peut perturber à la fois les capteurs et le module Wi-Fi, conduisant à une déconnexion et un redémarrage. **Résoudre le problème matériel I2C résout généralement le redémarrage.**

### 9) Valeurs de capteurs aberrantes (pics ou chutes soudaines)
**Symptômes** : Les graphiques montrent des pics soudains et irréalistes (ex: température qui saute à -140°C ou +80°C) ou l'humidité qui passe à 0% ou 100% brièvement.

**Cause probable** : Interférences électriques, souvent liées à l'ajout d'un module gourmand comme une **carte SD**.
- Les écritures sur carte SD génèrent des pics de consommation de courant.
- Ces pics peuvent créer de brèves chutes de tension sur le rail 3.3V.
- Les capteurs I2C sont sensibles à ces fluctuations et peuvent renvoyer des données corrompues.

**Solutions** :
1. **Alimentation 3.3V** : Si votre module SD le permet, alimentez-le en 3.3V plutôt qu'en 5V. Cela évite le régulateur interne du module (souvent bruyant) et aligne les niveaux logiques avec l'ESP32.
2. **Condensateur** : Ajoutez un condensateur de découplage (ex: 100µF) sur l'alimentation 3.3V près des capteurs ou du module SD.
3. **Logiciel** : Le firmware (v1.0.95+) intègre un filtre logiciel qui ignore ces valeurs aberrantes.

### 10) Crash de l'interface Web (Watchdog Timeout) et erreurs SD
**Symptômes** : L'appareil redémarre en essayant de lire un gros fichier (Logs, Historique) depuis l'interface Web. Les logs affichent `Task watchdog got triggered` sur la tâche `async_tcp`.

**Cause et Solution (Corrigé en v1.114+)** : Ce problème était dû à une opération bloquante dans le serveur web qui monopolisait le processeur lors du traitement de gros fichiers ou de flux de données (comme les graphiques d'historique). Cela déclenchait un timeout du "watchdog" de sécurité.

Depuis la version 1.114, le serveur web utilise une méthode de streaming entièrement non-bloquante qui rend la main au processeur pendant les opérations longues. Cela résout définitivement le problème. Si vous rencontrez ce bug, veuillez mettre à jour votre firmware vers la v1.114 ou une version plus récente.

## Workflow de mise à jour sûr
1. Sauvegarder les changements.
2. Compiler en local.
3. Uploader et observer la séquence de boot.
4. Vérifier les pages et les statuts LED.
5. Contrôler la page logs pour détecter les anomalies.
