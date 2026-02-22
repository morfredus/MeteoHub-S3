## Description alerte météo
L’API `/api/alert` retourne désormais le texte complet de l’alerte (en français si disponible). Le dashboard web affiche ce texte pour une clarté et une localisation maximale.

# Architecture du projet

Version minimale valide : 1.0.154

## Nouvelles fonctionnalités (depuis 1.0.127)
- **Cartouche alerte météo** : Le dashboard affiche en permanence une cartouche d’alerte météo (via `/api/alert`).
- **Données capteurs en temps réel** : L’endpoint `/api/live` retourne les valeurs réelles des capteurs (température, humidité, pression).
- **Dashboard vs Historique** : Le graphique du dashboard affiche les 2 dernières heures ; la page historique affiche 24h.
- **Conformité stricte au projet** : Toutes les routes API sont déclarées uniquement dans `_setupApi()`. Aucun commentaire interdit ou placeholder n’existe dans le code.
- **Amélioration OLED (>=1.0.139)** : Suppression du clignotement lors des transitions de page OLED grâce à la suppression du `show()` intermédiaire et à une gestion optimisée du buffer. Rendu plus fluide et stable sur SH1106.


## Objectif
Expliquer l’organisation du code source, la gestion des environnements OLED et LCD, et le flux de données dans le système.

## Structure des dossiers
- `src/main.cpp` : boot et orchestration principale pour l’environnement d’affichage sélectionné (OLED/LCD)
- `src/modules/` : modules matériel/affichage/pages (voir ci-dessous)
- `src/managers/` : gestionnaires d’état et d’orchestration
- `src/utils/` : utilitaires réutilisables
- `include/` : en-têtes de configuration réservés

## Gestion des environnements d’affichage
- Le type d’afficheur est choisi à la compilation (`esp32-s3-oled` ou `esp32-s3-lcd`).
- Toute la logique d’affichage/pages est abstraite via `DisplayInterface`.
- Modules dédiés pour chaque afficheur : `sh1106_display` (OLED), `st7789_display` (LCD).
- Logique de rendu des pages séparée : `pages_oled.cpp` (OLED), `pages_st7789.cpp` (LCD).
- Navigation adaptée : sur LCD, 1 cran = 1 page ; sur OLED, 2 crans = 1 page.

## Responsabilités des modules
### `src/modules/`
- `encoder` : décodage des entrées encodeur (abstraction matérielle)
- `neopixel_status` : sortie LED couleur/statut (OLED uniquement)
- `sensors` : acquisition AHT20 + BMP280
- `sh1106_display` : abstraction de rendu OLED
- `st7789_display` : abstraction de rendu LCD
- `pages_oled` : rendu des pages pour OLED
- `pages_st7789` : rendu des pages pour LCD

### `src/managers/`
- `wifi_manager` : retries Wi-Fi et état de connexion
- `forecast_manager` : récupération + parsing OpenWeatherMap
- `history` : buffer circulaire + persistance LittleFS
- `ui_manager` : gestion entrées, état pages/menu, logique de navigation (OLED/LCD)

### `src/utils/`
- `logs` : buffer circulaire de logs en mémoire + macros de log
- `system_info` : métriques mémoire/système runtime

## Flux de données runtime
1. `main.cpp` initialise le backend d’affichage sélectionné, les capteurs, le Wi-Fi et l’heure.
2. `ui_manager.update()` pilote la boucle périodique et adapte la navigation selon l’afficheur.
3. Les managers rafraîchissent les données (Wi-Fi, capteurs, prévisions, historique).
4. Les pages affichent l’état courant selon l’environnement d’affichage sélectionné (OLED ou LCD).
5. NeoPixel (OLED) ou alerte à l’écran (LCD) reflète l’état connexion/alertes.

## Modèle de persistance
- **NVS (Preferences)** : Stockage non-volatil pour les petites données (ex: index de la dernière page active).
- **LittleFS** : Système de fichiers sur la mémoire flash interne pour les données plus volumineuses.
  - **Historique récent** : Les dernières 24h sont ajoutées à un fichier (`/history/recent.dat`) pour minimiser l'usure de la flash.
  - **Rôle** : Sert de cache rapide pour le redémarrage et de stockage de secours pour les logs.
- **Carte SD** : Stockage principal pour l'archivage long terme.
  - **Format** : Fichiers CSV journaliers (`/history/YYYY-MM-DD.csv`).
  - **Avantage** : Capacité élevée, facilité de lecture sur PC, robustesse accrue par rapport à la flash interne pour les écritures fréquentes.

---
**[v1.0.119+] Robustesse carte SD**

Depuis la version 1.0.119, le formatage et le montage SD sont beaucoup plus robustes :
- Plusieurs tentatives de formatage à des vitesses SPI décroissantes (4MHz, 1MHz, 400kHz).
- Réinitialisation bas niveau entre chaque essai.
- Remontage et validation automatique après formatage.
- Au démarrage, le montage SD utilise des retries multi-fréquences (8MHz, 4MHz, 1MHz) pour tolérer les cartes instables et les montages sensibles.
Ceci améliore grandement la fiabilité avec les cartes SD problématiques et réduit le risque d'échec de montage.

## Dépendances externes
- Driver OLED (SH1106) et/ou LCD (Adafruit ST7789)
- Librairies capteurs Adafruit
- ArduinoJson
- WiFi / HTTPClient / LittleFS / Preferences
