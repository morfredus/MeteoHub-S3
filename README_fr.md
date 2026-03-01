# MeteoHub S3

> **Version minimale valide : 1.0.178**

## Documentation complète
- [Index de la documentation](docs/index_fr.md)

## Documentation Débutant
- [Guide Débutant (Français)](docs/beginner/index_fr.md)
- [Beginner's Guide (English)](docs/beginner/index.md)

## Présentation
MeteoHub S3 est un projet PlatformIO pour ESP32-S3 centré sur un tableau de bord OLED (SH1106/SSD1306 via U8g2). Il affiche les données capteurs locales, les prévisions météo, les logs et l’état système.

## Matériel nécessaire
- Écran OLED (SH1106 ou SSD1306, I2C)
- Module encodeur HW-040 (encodeur + bouton central)
- Boutons Back et Confirm
- Capteurs AHT20 et BMP280
- Carte SD optionnelle pour l’archivage long terme

## Compilation
- Installer PlatformIO dans VS Code
- Sélectionner l’environnement : `esp32-s3-oled` (ESP32-S3 DevKitC-1)
- Sélectionner l’environnement : `esp32-dev-oled` (ESP32 Dev Module)
- Sélectionner l’environnement : `esp32-dev-oled-ultra-safe` (ESP32 Dev Module mode debug SD ultra-safe)
- Build : `platformio run`
- Upload : `platformio run --target upload`

## Fonctionnalités principales
- Affichage graphique température, humidité, pression
- Trois modes d'échelle pour les graphes :
	1. Fixe : bornes définies dans la configuration (config.h), idéal pour comparer sur plusieurs jours.
	2. Dynamique : bornes calculées sur les données affichées, met en avant les variations récentes.
	3. Mixte : combine les deux, élargit l'échelle dynamique d'un pourcentage (modifiable), tout en restant dans les bornes fixes.
- Contrôle du mode et du pourcentage d'élargissement sur l'UI web
- Synchronisation automatique des paramètres entre config.h et l'UI web
- Aide contextuelle sous le graphique (explication des modes)
- Gestion SD robuste pour modules ESP32-S3 3.3V : fréquence SPI adaptative, remontage automatique, vérifications d’état et écritures sécurisées de l’historique

## Utilisation

Voir docs/user_guide_fr.md pour le détail des modes d'échelle.

---

Pour toute modification de config.h, les valeurs sont injectées dans l'UI web lors du build.
