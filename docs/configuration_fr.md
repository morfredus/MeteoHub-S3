## Description alerte météo
L’API `/api/alert` retourne désormais le texte complet de l’alerte (en français si disponible). Le dashboard web affiche ce texte pour une clarté et une localisation maximale.
# Configuration

Version minimale valide : 1.0.145

## Nouvelles fonctionnalités (depuis 1.0.127)
- **Cartouche alerte météo** : Le dashboard affiche en permanence une cartouche d’alerte météo (via `/api/alert`).
- **Données capteurs en temps réel** : L’endpoint `/api/live` retourne les valeurs réelles des capteurs (température, humidité, pression).
- **Dashboard vs Historique** : Le graphique du dashboard affiche les 2 dernières heures ; la page historique affiche 24h.
- **Conformité stricte au projet** : Toutes les routes API sont déclarées uniquement dans `_setupApi()`. Aucun commentaire interdit ou placeholder n’existe dans le code.


## Objectif
Configurer le réseau, l’API météo et les constantes runtime de manière sûre.

## Fichiers modifiables
### 1) `include/secrets.h`
Créer ce fichier à partir de `include/secrets_example.h`.

Utiliser ce fichier pour les données privées :
- Tableau d’identifiants Wi-Fi (`WIFI_CREDENTIALS`)
- Clé API OpenWeatherMap (`OWM_API_KEY`)
- Coordonnées (`OWM_LAT`, `OWM_LON`)
- Unités et langue (`OWM_UNITS`, `OWM_LANG`)

### 2) `include/config.h`
Utiliser ce fichier pour les réglages fonctionnels :
- Intervalle de rafraîchissement dashboard (`DASHBOARD_REFRESH_MS`)
- Délai de retry Wi-Fi (`WIFI_RETRY_DELAY_MS`)
- Contraste OLED (`OLED_CONTRAST`)
- Activation du test ping (`ENABLE_PING_TEST`)
- Nom réseau local (`WEB_MDNS_HOSTNAME`) : par défaut "meteohub" (accessible via `http://meteohub.local`).


## Environnements OLED/LCD
Le firmware prend en charge l’OLED SH1106/SSD1306 et le LCD ST7789. Le type d’afficheur est choisi à la compilation (`esp32-s3-oled` / `esp32-s3-lcd`), et le contrôleur/adresse I2C OLED se configurent dans `include/config.h`. Voir guide utilisateur et câblage matériel pour le détail.

## Fichier à ne pas modifier à la légère
### `include/board_config.h`
Ce fichier définit le mapping GPIO et ne doit être modifié qu’après décision matérielle.

## Métadonnées version projet
Le nom et la version sont injectés depuis `platformio.ini` via build flags :
- `PROJECT_NAME`
- `PROJECT_VERSION`

Ne pas coder en dur le nom/la version ailleurs.

## Conseils de sécurité
- `include/secrets.h` doit être créé à partir de `include/secrets_example.h`.
- Ne jamais pousser `include/secrets.h` sur GitHub.
- Ne jamais partager `include/secrets.h` en dehors de votre environnement local de confiance.

## Vérification rapide de configuration
- La compilation passe (`platformio run`)
- Le Wi-Fi se connecte au boot
- La page prévisions affiche des données
- La page logs montre les événements réseau/météo
