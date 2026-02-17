
# Journal des modifications du projet

Version minimale valide : 1.0.60

## Version 1.0.75
- **Documentation** : Mise à jour globale de la documentation (README, Guides, FAQ) pour inclure le support LCD 240x320 et les nouveaux écrans de démarrage. Synchronisation bilingue.

## Version 1.0.74
- **Correctif (OLED)** : Ajout de l'inclusion manquante `sh1106_display.h` dans `pages_sh1106.cpp` pour résoudre l'erreur de compilation liée à `Sh1106Display`.

## Version 1.0.73
- **UI (Boot)** : Ajustement esthétique du Splash Screen LCD : "morfredus" est désormais en minuscules et parfaitement centré verticalement entre les lignes décoratives.

## Version 1.0.72
- **Correctif (Compilation)** : Ajout des déclarations manquantes pour `drawSplashScreen_st7789` et `drawBootProgress_st7789` dans le fichier d'en-tête, résolvant l'erreur de compilation sur l'environnement LCD.

## Version 1.0.71
- **Correctif (Main)** : Intégration effective des appels aux nouveaux écrans de démarrage (Splash Screen et Boot Progress) dans la séquence d'initialisation principale, remplaçant l'ancienne logique statique.

## Version 1.0.70
- **Correctif (Dev)** : Ajout des déclarations manquantes dans les fichiers d'en-tête pour les fonctions de Splash Screen et de Boot Progress.

## Version 1.0.69
- **UI (Boot)** : Simplification du logo de démarrage ("MORFREDUS" uniquement).
- **UI (OLED)** : Implémentation des écrans de démarrage et de progression pour l'environnement OLED (SH1106).

## Version 1.0.68
- **UI (Boot)** : Ajout d'un écran de démarrage animé "Morfredus" et d'une barre de progression visuelle pour les étapes d'initialisation (Capteurs, WiFi, NTP, etc.), adaptatifs à la résolution LCD.

## Version 1.0.67
- **Documentation** : Ajout des entrées manquantes dans le journal des modifications pour les versions 1.0.61 à 1.0.64 (Support LCD 240x320).

## Version 1.0.66
- **Correctif (UI)** : Ajustement vertical (+2px) de l'affichage de la température sur l'écran d'accueil en mode 240x240 pour éviter le chevauchement avec le titre.

## Version 1.0.65
- **Correctif (UI)** : Ajustement de la position verticale des graphiques sur écran LCD. Le haut du graphique a été décalé vers le bas pour éviter que les étiquettes de l'axe Y ne chevauchent la ligne de séparation de l'en-tête.

## Version 1.0.64
- **Fonctionnalité (UI)** : Adaptation réactive de toutes les pages (Réseau, Système, Logs, Météo, Graphes, Prévisions) pour supporter la résolution verticale étendue (320px) tout en préservant l'affichage 240px.

## Version 1.0.63
- **Pilote (LCD)** : Le driver `St7789Display` utilise désormais les dimensions dynamiques définies dans la configuration globale.

## Version 1.0.62
- **Configuration** : Nettoyage des définitions de version dans `platformio.ini`.

## Version 1.0.61
- **Configuration** : Introduction des constantes `LCD_WIDTH` et `LCD_HEIGHT` dans `config.h` pour paramétrer la résolution de l'écran.

## Version 1.0.60
- **Correctif (LCD)** : Correction de l'échelle des graphiques sur écran ST7789. La courbe s'étire désormais dynamiquement sur toute la largeur de l'écran, garantissant que la dernière mesure est bien calée à droite.
- **Refactorisation (UI)** : Simplification de la logique de l'encodeur rotatif. L'écran LCD utilise désormais le sens naturel (Horaire = Suivant), tandis que l'inversion spécifique à l'OLED est gérée de manière centralisée.

## Version 1.0.59
- **Correctif** : Résolution d'un bug critique où les pages de l'interface utilisateur ne se chargeaient pas ou ne s'affichaient pas dans l'environnement LCD (ST7789). La logique de dessin des pages est maintenant correctement implémentée pour l'OLED et le LCD.

## Version 1.0.58
- **Gestion d'historique robuste** : Refonte complète du système d'historique pour une fiabilité à long terme.
  - **Stockage optimisé pour la Flash** : L'historique récent est désormais ajouté à un fichier (et non réécrit), réduisant drastiquement l'usure de la mémoire flash.
  - **Données instantanées au démarrage** : Le système recharge en RAM les dernières 24h de données haute résolution et 30 jours de résumés quotidiens au démarrage.
  - **Cohérence des interfaces** : Toutes les UI (OLED, LCD, Web) ont un accès immédiat à l'historique complet, éliminant les "graphes vides" après un redémarrage.
  - **Intégrité des données** : Ajout d'une vérification pour empêcher la sauvegarde de points avant la synchronisation de l'heure NTP.
  - **Maintenance automatique** : Les archives de plus de 2 ans sont désormais automatiquement supprimées.

## Version 1.0.57
- **Correctif** : Résolution d'un problème critique où l'historique des mesures (sauvegarde, lecture, récupération après redémarrage) ne fonctionnait pas dans l'environnement LCD (ST7789). La fonctionnalité est maintenant unifiée et stable sur les environnements OLED et LCD.

## Version 1.0.51
- **Documentation** : Mise à jour complète des guides utilisateur, FAQ et maintenance pour inclure l'interface Web et la nouvelle gestion d'historique.

## Versions 1.0.30 à 1.0.50 (Mise à jour majeure Web & Historique)
- **Interface Web** : Tableau de bord complet accessible via `http://meteohub.local` (Live, Graphes, Stats, Fichiers).
- **Gestionnaire de Fichiers** : Interface Web pour gérer les fichiers LittleFS (Logs, Historique).
- **Historique Hybride** : Stockage haute résolution (RAM) pour 24h et archivage mensuel décimé (LittleFS) pour le long terme (2 ans).
- **Graphiques Web** : Visualisation multi-axes (Temp/Hum/Pres) avec couleurs adaptées.
- **OLED** : Correction de la navigation (sens encodeur) et du menu pour l'écran SH1106.
- **Système** : Uniformisation des noms de GPIO (`_PIN`) et optimisations diverses.

## Version 1.0.29
- **Interface LCD** : Refonte complète de l'interface ST7789 (thème sombre, en-têtes colorés, cartes de données).
- **Anti-scintillement** : Implémentation du double buffering pour l'écran LCD.
- **Navigation** : Inversion du sens de l'encodeur sur LCD (Pages et Menu) pour une logique plus naturelle (Horaire = Suivant).
- **Menu Système** : Redesign du menu sur LCD pour s'aligner avec la charte graphique.
- **Ordre des pages** : Inversion des pages "Système" et "Logs" sur LCD.
- **Correctifs** : Résolution de l'erreur de liaison `clearLogs`, correction du gel d'écran et gestion des accents.

## Version 1.0.28
- **Affichage prévisions** : La page prévisions affiche désormais les vraies valeurs (température, min, max) sur LCD.
- **Graphiques LCD** : Les axes et valeurs sont décalés pour éviter le chevauchement sur LCD.
- **Encodeur rotatif LCD** : Sur LCD, 1 cran = 1 page (logique OLED inchangée).
- **API encodeur** : Méthode publique pour le nombre de crans, plus d'accès direct au rotary privé.
- **Docs & Build** : Documentation et code synchronisés, compilation et upload validés.

## Version 1.0.27
- **Initialisation I2C** : Initialisation dynamique du bus I2C dans SensorManager pour OLED et LCD, assurant la détection AHT20/BMP280 sur toutes les cartes.
- **Correctif capteurs LCD** : Les capteurs fonctionnent désormais sans modification sur les versions LCD (broches I2C par défaut, personnalisables dans board_config.h).
- **Rotation écran LCD** : L’orientation par défaut de l’écran LCD est maintenant retournée (setRotation(2)) pour un affichage correct.

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
