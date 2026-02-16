
# Architecture du projet

Version minimale valide : 1.0.30

## Objectif
Expliquer l’organisation du code source, la gestion des environnements OLED et LCD, et le flux de données dans le système.

## Structure des dossiers
- `src/main.cpp` : boot et orchestration principale, auto-détection du type d’afficheur (OLED/LCD)
- `src/modules/` : modules matériel/affichage/pages (voir ci-dessous)
- `src/managers/` : gestionnaires d’état et d’orchestration
- `src/utils/` : utilitaires réutilisables
- `include/` : en-têtes de configuration réservés

## Gestion des environnements d’affichage
- Le firmware détecte automatiquement au démarrage si un OLED SH1106 ou un LCD TFT ST7789 est connecté.
- Toute la logique d’affichage/pages est abstraite via `DisplayInterface`.
- Modules dédiés pour chaque afficheur : `sh1106_display` (OLED), `st7789_display` (LCD).
- Logique de rendu des pages séparée : `pages_sh1106.cpp` (OLED), `pages_st7789.cpp` (LCD).
- Navigation adaptée : sur LCD, 1 cran = 1 page ; sur OLED, 2 crans = 1 page.

## Responsabilités des modules
### `src/modules/`
- `encoder` : décodage des entrées encodeur (abstraction matérielle)
- `neopixel_status` : sortie LED couleur/statut (OLED uniquement)
- `sensors` : acquisition AHT20 + BMP280
- `sh1106_display` : abstraction de rendu OLED
- `st7789_display` : abstraction de rendu LCD
- `pages_sh1106` : rendu des pages pour OLED
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
1. `main.cpp` initialise l’affichage (auto-détection), les capteurs, le Wi-Fi, l’heure.
2. `ui_manager.update()` pilote la boucle périodique et adapte la navigation selon l’afficheur.
3. Les managers rafraîchissent les données (Wi-Fi, capteurs, prévisions, historique).
4. Les pages affichent l’état courant sur l’afficheur détecté (OLED ou LCD).
5. NeoPixel (OLED) ou alerte à l’écran (LCD) reflète l’état connexion/alertes.

## Modèle de persistance
- NVS (Preferences) : index de dernière page active
- LittleFS : enregistrements historiques des capteurs

## Dépendances externes
- Driver OLED (SH1106) et/ou LCD (Adafruit ST7789)
- Librairies capteurs Adafruit
- ArduinoJson
- WiFi / HTTPClient / LittleFS / Preferences
