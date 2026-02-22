# Prise en main

Version minimale valide : 1.0.154

## Objectif
Ce guide aide un débutant à démarrer MeteoHub S3 depuis zéro jusqu’au premier upload réussi.


## Environnements supportés
- **OLED (SH1106, I2C)** : Monochrome, 128x64, encodeur rotatif + 2 boutons, NeoPixel, capteurs AHT20/BMP280.
- **LCD (TFT ST7789, SPI)** : Couleur, 240x240 ou 240x320, encodeur rotatif (HW-040) + 3 boutons, capteurs AHT20/BMP280.

Choisissez le type d’afficheur à la compilation via `esp32-s3-oled` ou `esp32-s3-lcd` ; le firmware applique ensuite l’interface et la navigation correspondantes. Voir [Câblage matériel](hardware_wiring_fr.md) pour le détail des broches.


## Matériel nécessaire

- **Carte ESP32-S3-DevKitC-1-N16R8V**
- **OLED (HW-040 complet)** :
	- PCB avec OLED intégré
	- Encodeur rotatif central (bouton intégré)
	- Boutons Back et Confirm soudés sur le PCB
	- NeoPixel intégré
- **LCD (EC11 seul)** :
	- Écran TFT ST7789 (240x240 ou 240x320)
	- Encodeur rotatif EC11 (bouton central)
	- Boutons Back et Confirm à câbler séparément
- **Capteurs** : AHT20 et/ou BMP280
- **Câblage** : fils Dupont, connecteurs, breadboard ou soudure
- **Alimentation USB** : câble compatible données, alimentation stable
- **PC Windows**
- **Visual Studio Code**
- **Extension PlatformIO**

L’environnement OLED (HW-040) est le plus simple à assembler (tout intégré sur le PCB). L’environnement LCD nécessite le câblage manuel des boutons Back/Confirm.

## 1) Ouvrir le projet
1. Ouvrir VS Code.
2. Ouvrir le dossier : `Dashboard-IoT`.
3. Attendre l’indexation du projet par PlatformIO.

## 2) Préparer le fichier secrets
1. Créer `include/secrets.h` en copiant `include/secrets_example.h`.
2. Renseigner les identifiants Wi-Fi (`WIFI_CREDENTIALS`).
3. Renseigner la clé API OpenWeatherMap (`OWM_API_KEY`).
4. Vérifier les coordonnées (`OWM_LAT`, `OWM_LON`) et les unités (`OWM_UNITS`).
5. (Optionnel) Pour l'écran LCD, ajuster `LCD_WIDTH` et `LCD_HEIGHT` dans `include/config.h` si vous utilisez un écran 240x320.

## Règle de sécurité secrets (obligatoire)
- `include/secrets.h` est un fichier privé.
- Ne jamais pousser `include/secrets.h` sur GitHub.
- Ne jamais partager `include/secrets.h` (chat, capture d’écran, email, archive de dépôt).

## 3) Compiler le firmware
Exécuter dans le terminal :
- `platformio run`

Résultat attendu :
- La compilation se termine avec `[SUCCESS]`.

## 4) Uploader sur la carte
Exécuter :
- `platformio run --target upload`

Résultat attendu :
- Le firmware est flashé sans erreur d’upload.

## 5) Vérifier le comportement
Après redémarrage, l’écran OLED doit afficher les phases de boot :
- Splash Screen "morfredus"
- Initialisation capteurs
- Connexion Wi-Fi
- Synchronisation de l’heure
- Système prêt


## Problèmes fréquents au premier démarrage
- **Pas de Wi-Fi** : vérifier SSID/mot de passe dans `include/secrets.h`.
- **Pas de prévision** : vérifier `OWM_API_KEY` et la connexion internet.
- **Pas d’affichage** : vérifier le câblage SDA/SCL et l’alimentation OLED.
- **Échec upload** : vérifier le câble USB et le port COM sélectionné.

> **En cas de blocage ou de redémarrages en boucle**
>
> Si l’appareil ne démarre plus ou reste bloqué sur l’écran de démarrage, il est possible de forcer un formatage d’urgence de la mémoire interne (LittleFS) :
> 1. Débranchez l’alimentation USB.
> 2. Maintenez le bouton **BOOT** enfoncé.
> 3. Rebranchez l’alimentation tout en gardant le bouton BOOT appuyé.
> 4. Attendez le message de maintenance à l’écran, puis relâchez le bouton.
> 5. L’appareil va effacer la mémoire et redémarrer proprement (toutes les données historiques seront perdues).

Cette procédure ne nécessite aucun outil ni logiciel, et permet à tout utilisateur de repartir sur une base saine en cas de problème.

## Étape suivante
Continuer avec [Câblage matériel](hardware_wiring_fr.md) et [Configuration](configuration_fr.md).
