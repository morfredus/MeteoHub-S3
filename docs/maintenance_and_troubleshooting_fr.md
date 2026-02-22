# Maintenance et dépannage

Version minimale valide : 1.0.145

## Objectif
Fournir des étapes pratiques de diagnostic et de reprise quand le système ne se comporte pas comme prévu.

## Vérifications build et upload
- Build : `platformio run`
- Upload : `platformio run --target upload`
- Nettoyage cache : `platformio run --target clean`

## Problèmes typiques
### 1) Le Wi-Fi ne se connecte jamais
Vérifiez les identifiants dans `include/secrets.h`, la disponibilité du routeur et la qualité du signal.

### 2) La page prévisions est vide
Vérifiez la clé OpenWeather, l’accès internet, les coordonnées, et le quota API.

### 3) L’écran OLED reste noir
Vérifiez le câblage SDA/SCL, l’alimentation OLED et la compatibilité SH1106.

### 4) Capteurs invalides ou absents
Vérifiez le câblage AHT20/BMP280, la stabilité I2C et la qualité d’alimentation.

### 5) Les graphes sont vides
L’historique est rechargé automatiquement au démarrage. Si ce n’est pas le cas, effacez l’historique depuis le menu système puis redémarrez.

### 6) Interface Web inaccessible
- Vérifiez que vous êtes sur le même réseau Wi-Fi.
- Essayez l’IP directe si `meteohub.local` ne répond pas.
- Vérifiez les logs de démarrage (montage LittleFS).

### 7) Boucle de redémarrage / freeze au boot (watchdog)
Symptômes : redémarrages en boucle, `Task watchdog got triggered`, erreurs d’ouverture de fichiers.

**Cause fréquente :** incohérence du système de fichiers après coupure brutale.

**Récupération d’urgence (simple) :**
1. Débranchez l’USB.
2. Maintenez **BOOT**.
3. Rebranchez l’USB en gardant BOOT appuyé.
4. Attendez le message de maintenance/formatage puis relâchez.
5. L’appareil formate la mémoire interne puis redémarre.

### 8) Erreurs I2C (`i2cRead returned Error -1`) et pics de mesures
Cause probable : câblage/alimentation.
- Vérifiez SDA/SCL/VCC/GND,
- testez un autre câble/alimentation USB,
- avec module SD, privilégiez une alimentation 3.3V stable et ajoutez un condensateur de découplage.

## Actions de maintenance disponibles dans le menu UI
- Reboot
- Effacer les logs
- Effacer l’historique
- Formater la SD (si présente)

## Procédure de mise à jour sûre
1. Sauvegarder les changements de configuration/code.
2. Compiler.
3. Téléverser et observer les logs de démarrage.
4. Vérifier les pages et indicateurs.
5. Vérifier l’interface Web, les logs et l’historique.
