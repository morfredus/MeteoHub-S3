

# MeteoHub S3

Version minimale valide : 1.0.121

## Documentation complète
- [Index de la documentation](docs/index_fr.md)

## Présentation
MeteoHub S3 est un projet PlatformIO pour ESP32-S3 qui affiche les données des capteurs locaux, les prévisions météo, les logs et l’état système sur un écran couleur LCD (TFT ST7789) ou un tableau de bord OLED SH1106. Le firmware détecte automatiquement le type d’afficheur et adapte l’interface et la navigation.


## Matériel nécessaire et différences

- **OLED (SH1106, I2C)** :
	- Module HW-040 complet : PCB avec OLED intégré, encodeur rotatif (bouton central) et boutons Back/Confirm directement soudés.
	- Navigation : 1 cran = 1 page.
	- NeoPixel intégré.
	- Capteurs AHT20/BMP280.

- **LCD (TFT ST7789, SPI)** :
	- Module EC11 seul : encodeur rotatif (sans boutons Back/Confirm sur le PCB, à câbler séparément).
	- Définitions acceptées : 240x240 ou 240x320 (configurable dans `include/config.h`).
	- Navigation : 2 crans = 1 page.
	- Capteurs AHT20/BMP280.

Les deux environnements sont détectés automatiquement au démarrage et l'interface s'adapte.

## Architecture du projet
- `src/modules/` : modules matériel/affichage/pages (`encoder`, `neopixel_status`, `sensors`, `sh1106_display`, `st7789_display`, `pages`)
- `src/managers/` : gestionnaires d’orchestration et d’état (`wifi_manager`, `ui_manager`, `history`, `sd_manager`, `web_manager`)
- `src/utils/` : code utilitaire réutilisable (`logs`, `system_info`)
- `include/` : uniquement les en-têtes de configuration réservés (`board_config.h`, `config.h`, `secrets.h`, `secrets_example.h`, `README`)

## Compilation
- Installer PlatformIO dans VS Code.
- Sélectionner l’environnement `esp32-s3-lcd` (TFT) ou `esp32-s3-oled` (OLED) selon le matériel.
- Commande de build : `platformio run`
- Commande d’upload : `platformio run --target upload`

## Fonctionnalités principales

- **Gestion avancée de la carte SD**
	- La carte SD devient le support principal pour l’archivage long terme (fichiers CSV quotidiens)
	- Gestionnaire de fichiers Web permettant la navigation, le téléchargement, l’upload et la suppression sur SD et mémoire interne
	- **Formatage de la carte SD** directement depuis le menu système (avec confirmation et retour visuel)
	- Création et gestion automatique du dossier `/history` sur la SD

- **Sauvegarde robuste & historique**
	- Buffer RAM circulaire 24h, persistance LittleFS (flash interne) pour redémarrage rapide
	- Archivage automatique sur SD pour la conservation long terme
	- Filtrage automatique des valeurs aberrantes (capteurs)
	- Effacement sélectif des logs et de l’historique (avec confirmation)

- **Interface Web enrichie**
	- Tableau de bord temps réel, graphiques interactifs, statistiques et historique long terme
	- Page logs système avec rafraîchissement automatique et coloration (Info/Warn/Error)
	- Footer et menu partagés pour une navigation cohérente sur toutes les pages
	- Gestionnaire de fichiers multi-support (LittleFS/SD)

- **Expérience utilisateur & sécurité**
	- Navigation et gestion des boutons unifiées entre OLED et LCD
	- Écrans de confirmation pour les actions critiques (formatage SD, effacement logs/historique)
	- Retour visuel pour toutes les opérations de maintenance

- **Documentation**
	- Toute la documentation utilisateur (EN/FR) est strictement synchronisée avec le code et les fonctionnalités
	- La version minimale requise est toujours indiquée


## LCD vs OLED : Différences clés
- **Navigation** : Sur OLED, 1 cran = 1 page (module HW-040 complet, boutons + OLED intégré) ; sur LCD, 2 crans = 1 page (module EC11 seul).
- **Affichage** : LCD en couleur, haute résolution, supporte des graphismes avancés (axes, courbes, etc.).
- **Câblage** : Voir [Câblage matériel](docs/hardware_wiring_fr.md) pour le détail des broches selon l’environnement.

Pour l’installation, l’utilisation et le dépannage détaillés, consultez la documentation utilisateur dans le dossier `docs/`.
