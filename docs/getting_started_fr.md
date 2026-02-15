# Prise en main

Version minimale valide : 1.0.21

## Objectif
Ce guide aide un débutant à démarrer MeteoHub S3 depuis zéro jusqu’au premier upload réussi.

## Prérequis
- Un PC Windows
- Visual Studio Code
- L’extension PlatformIO dans VS Code
- Une carte ESP32-S3-DevKitC-1-N16R8V
- Un câble USB compatible données

## 1) Ouvrir le projet
1. Ouvrir VS Code.
2. Ouvrir le dossier : `Dashboard-IoT`.
3. Attendre l’indexation du projet par PlatformIO.

## 2) Préparer le fichier secrets
1. Créer `include/secrets.h` en copiant `include/secrets_example.h`.
2. Renseigner les identifiants Wi-Fi (`WIFI_CREDENTIALS`).
3. Renseigner la clé API OpenWeatherMap (`OWM_API_KEY`).
4. Vérifier les coordonnées (`OWM_LAT`, `OWM_LON`) et les unités (`OWM_UNITS`).

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
- Booting
- Initialisation capteurs
- Connexion Wi-Fi
- Synchronisation de l’heure
- Système prêt

## Problèmes fréquents au premier démarrage
- **Pas de Wi-Fi** : vérifier SSID/mot de passe dans `include/secrets.h`.
- **Pas de prévision** : vérifier `OWM_API_KEY` et la connexion internet.
- **Pas d’affichage** : vérifier le câblage SDA/SCL et l’alimentation OLED.
- **Échec upload** : vérifier le câble USB et le port COM sélectionné.

## Étape suivante
Continuer avec [Câblage matériel](hardware_wiring_fr.md) et [Configuration](configuration_fr.md).
