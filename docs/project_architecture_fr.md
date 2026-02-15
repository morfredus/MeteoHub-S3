# Architecture du projet

Version minimale valide : 1.0.20

## Objectif
Expliquer l’organisation du code source et le flux de données dans le système.

## Structure des dossiers
- `src/main.cpp` : boot et orchestration principale
- `src/modules/` : modules matériel/affichage/pages
- `src/managers/` : gestionnaires d’état et d’orchestration
- `src/utils/` : utilitaires réutilisables
- `include/` : en-têtes de configuration réservés

## Responsabilités des modules
### `src/modules/`
- `encoder` : décodage des entrées encodeur
- `neopixel_status` : sortie LED couleur/statut
- `sensors` : acquisition AHT20 + BMP280
- `sh1106_display` : abstraction de rendu OLED
- `pages` : logique de rendu des pages

### `src/managers/`
- `wifi_manager` : retries Wi-Fi et état de connexion
- `forecast_manager` : récupération + parsing OpenWeatherMap
- `history` : buffer circulaire + persistance LittleFS
- `ui_manager` : gestion entrées + état pages/menu

### `src/utils/`
- `logs` : buffer circulaire de logs en mémoire + macros de log
- `system_info` : métriques mémoire/système runtime

## Flux de données runtime
1. `main.cpp` initialise affichage, capteurs, Wi-Fi, heure.
2. `ui_manager.update()` pilote la boucle périodique.
3. Les managers rafraîchissent les données (Wi-Fi, capteurs, prévisions, historique).
4. Les pages affichent l’état courant sur OLED.
5. Le NeoPixel reflète l’état connexion/alertes.

## Modèle de persistance
- NVS (Preferences) : index de dernière page active
- LittleFS : enregistrements historiques des capteurs

## Dépendances externes
- Driver OLED (SH1106)
- Librairies capteurs Adafruit
- ArduinoJson
- WiFi / HTTPClient / LittleFS / Preferences
