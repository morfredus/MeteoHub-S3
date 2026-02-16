# Journal des modifications du projet

Version minimale valide : 1.0.26
## Version 1.0.26
- **Multi-écrans** : Ajout d'un environnement LCD (TFT ST7789) avec gestion conditionnelle du code et du brochage.
- **Protection compilation** : Ajout de conditions dans tous les fichiers d'affichage pour éviter les conflits entre OLED et LCD.
- **Documentation** : Mise à jour de tous les documents utilisateur pour refléter la nouvelle architecture.

## Version 1.0.25
- **Correctif échelle graphes** : Les valeurs min/max des graphes Température, Humidité et Pression sont désormais calculées sur l’historique affiché.
- **Lisibilité pression** : Les libellés latéraux utilisent une précision adaptative pour éviter un min/max visuellement identique à cause de l’arrondi.

## Version 1.0.24
- **Documentation utilisateur** : Ajout d’un guide pratique pour lire les valeurs Température, Humidité et surtout Pression.
- **Interprétation météo** : Documentation de la lecture des tendances de pression et du croisement Temp/Hum/Pression pour estimer l’évolution météo à court terme.

## Version 1.0.23
- **Priorisation des alertes** : La sélection d’alerte météo évalue désormais toutes les alertes reçues et conserve la plus critique.
- **Priorité crues** : Les événements crues/inondations sont explicitement priorisés devant les événements pluie à sévérité égale.
- **Traduction affichage** : La traduction des libellés d’alerte reconnaît aussi les mots-clés français `crue`, `inondation` et `pluie`.

## Version 1.0.22
- **Logique d’alerte** : Amélioration de la visualisation des alertes météo avec des couleurs NeoPixel distinctes Jaune/Orange/Rouge selon les mots-clés de description d’événement.

## Version 1.0.21
- **Documentation sécurité** : Précision que `include/secrets.h` doit être créé à partir de `include/secrets_example.h`, ne doit jamais être poussé sur GitHub et ne doit jamais être partagé.

## Version 1.0.20
- **Documentation** : Ajout d’un jeu complet de documentation orientée débutant dans `docs/`.
- **Cohérence bilingue** : Ajout d’équivalents stricts en anglais et en français pour tous les nouveaux documents utilisateur.
- **Guides projet** : Documentation des workflows d’installation, câblage, configuration, utilisation, architecture et dépannage.

## Version 1.0.19
- **Discipline mémoire** : Remplacement de la gestion texte principale par `std::string` dans les managers, pages et logs.
- **Interface affichage** : Mise à jour de l’API `Sh1106Display` pour accepter `std::string` et centralisation de la conversion à la frontière du driver écran.
- **Cohérence code** : Harmonisation de l’ordre des includes dans les fichiers mis à jour selon les règles du projet.

## Version 1.0.18
- **Architecture** : Réorganisation des fichiers sources dans `src/modules`, `src/managers` et `src/utils`.
- **Cohérence build** : Mise à jour de tous les chemins d’include locaux pour correspondre à la nouvelle structure.
- **Hygiène du projet** : Suppression des doublons `forecast_manager` à la racine et restriction de `include/` aux fichiers réservés.
- **Logging** : Standardisation des logs d’exécution via les macros `LOG_DEBUG`, `LOG_INFO`, `LOG_WARNING` et `LOG_ERROR`.

## Version 1.0.17
- **Correctifs** : Résolution d’erreurs de compilation liées à la capitalisation de `String` et correction de conflits de broches des boutons.

## Version 1.0.16
- **Améliorations UX** : Réorganisation des pages pour un flux logique (Météo -> Prévisions -> Graphes) et harmonisation des en-têtes de page.

## Version 1.0.15
- **Interactivité** : Ajout du défilement par clic pour les vues de la page Prévisions.

## Version 1.0.14
- **Localisation** : Ajout de traductions françaises pour certains événements d’alerte météo.

## Version 1.0.13
- **Identité projet** : Renommage du projet en "MeteoHub S3" et injection dynamique de la version via les build flags PlatformIO.

## Version 1.0.12
- **Menu système** : Ajout d’un menu contextuel pour les actions de maintenance (Redémarrer, Effacer les logs, Effacer l’historique).

## Version 1.0.11
- **LED d’état** : Implémentation de la logique NeoPixel pour le retour visuel (Vert : WiFi OK, Rouge : WiFi/Température alerte, Orange : Alerte météo).

## Version 1.0.10
- **Prévisions météo** : Intégration de l’API OpenWeatherMap (`ForecastManager`) pour récupérer les prévisions Aujourd’hui/Demain et les alertes.

## Version 1.0.9
- **Moteur de graphes** : Ajout de graphes dynamiques Température, Humidité et Pression avec auto-ajustement des axes.

## Version 1.0.8
- **Persistance** : Ajout du stockage LittleFS pour l’historique et des préférences (NVS) pour mémoriser la dernière page active après redémarrage.

## Version 1.0.7
- **Historique des données** : Implémentation de `HistoryManager` avec buffer circulaire pour stocker 128 points.

## Version 1.0.6
- **Système UI** : Développement d’une interface par pages (`UiManager`) navigable avec encodeur rotatif.

## Version 1.0.5
- **Séquence de démarrage** : Création d’un écran d’accueil animé avec barre de progression affichant les étapes d’initialisation (Capteurs, WiFi, Heure).

## Version 1.0.4
- **Connectivité** : Ajout de `WifiManager` avec logique de connexion non bloquante et synchronisation horaire NTP.

## Version 1.0.3
- **Driver capteurs** : Implémentation de `SensorManager` pour AHT20 et BMP280.

## Version 1.0.2
- **Configuration matérielle** : Mapping des GPIO spécifiques pour ESP32-S3 (OLED, encodeur, boutons, NeoPixel).

## Version 1.0.1
- **Optimisation encodeur rotatif** : Ajustement de la résolution à 4 impulsions par cran et amélioration de l’anti-rebond pour une navigation fiable.
