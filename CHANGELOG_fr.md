
# Journal des modifications du projet

Version minimale valide : 1.0.93

## Version 1.0.104
- **Documentation** : Révision complète des guides utilisateur et FAQ pour refléter précisément la stratégie de stockage hybride (SD principale, LittleFS cache) et les fonctionnalités actuelles de l'interface Web.

## Version 1.0.103
- **Documentation** : Mise à jour globale et vérification de la documentation utilisateur (Guides, FAQ, Architecture) pour garantir la cohérence avec les dernières fonctionnalités (Web UI, SD, Logs).

## Version 1.0.102
- **Web UI** : Harmonisation du pied de page sur toutes les pages avec une icône unique "Disquette" pour le gestionnaire de fichiers et "Parchemin" pour les logs.
- **UI (OLED)** : Ajout de la fonctionnalité de défilement des logs sur l'écran OLED via le bouton "Confirm".

## Version 1.0.101
- **Refactorisation (Web)** : Centralisation du code du pied de page dans `footer.js` pour une maintenance aisée et une cohérence sur toutes les pages.
- **Fonctionnalité (Web)** : Ajout de boutons dans le gestionnaire de fichiers pour basculer facilement entre la mémoire interne et la carte SD.

## Version 1.0.100
- **Web UI (Logs)** : Nouvelle page `/logs` avec coloration syntaxique (Info/Warn/Error) et rafraîchissement automatique.
- **Web UI (Fichiers)** : Amélioration majeure du gestionnaire de fichiers avec navigation dans les dossiers, fil d'ariane et support complet de la carte SD.
- **Web UI (Accès)** : Ajout d'une icône "Disquette" pour un accès direct aux fichiers SD depuis le pied de page.

## Version 1.0.99
- **Fonctionnalité (Web)** : Ajout d'une page `/logs` accessible via une icône dans le pied de page pour consulter les logs système en temps réel.
- **Correctif (SD)** : Vérification automatique de l'existence du dossier `/history` avant chaque écriture pour éviter les erreurs après un formatage ou un changement de carte.

## Version 1.0.98
- **UI (Sécurité)** : Ajout d'écrans de confirmation pour les actions "Clear Logs" et "Clear History" dans le menu système, évitant les suppressions accidentelles.
- **Web UI** : Simplification du raccourci d'accès à la carte SD vers `/sd` (redirige vers `/files.html?fs=sd`).

## Version 1.0.97
- **Amélioration (Web)** : Ajout d'un raccourci `/sd-files` redirigeant vers le gestionnaire de fichiers en mode SD (`/files.html?fs=sd`) et affichage du lien dans les logs au démarrage.
- **Documentation** : Mise à jour des conseils sur l'alimentation (3.3V vs 5V pour le module SD).

## Version 1.0.96
- **Documentation** : Ajout d'une note de dépannage sur les interférences possibles entre la carte SD (pics de consommation) et les capteurs I2C, expliquant l'origine des valeurs aberrantes.

## Version 1.0.95
- **Correctif (Capteurs)** : Ajout d'un filtrage des valeurs aberrantes (température, humidité, pression) avant l'enregistrement dans l'historique pour éviter de polluer les données avec des erreurs de lecture.
- **Amélioration (Web)** : Ajout d'une redirection `/files` vers `/files.html` et affichage de l'URL du gestionnaire de fichiers dans les logs au démarrage pour faciliter l'accès.

## Version 1.0.94
- **Correctif (Compilation)** : Synchronisation du fichier d'en-tête `web_manager.h` avec son implémentation pour résoudre les erreurs de compilation liées à la gestion de la carte SD dans l'interface web.

## Version 1.0.93
- **Fonctionnalité (Stockage)** : La carte SD est désormais le support de stockage principal pour l'historique haute résolution, avec des fichiers CSV créés quotidiennement (`/history/YYYY-MM-DD.csv`).
- **Fonctionnalité (Web UI)** : Le gestionnaire de fichiers web permet maintenant de naviguer et gérer les fichiers sur la carte SD en plus de la mémoire interne.
- **Stabilité** : L'archivage sur LittleFS est conservé comme un cache de 24h pour un redémarrage rapide.
## Version 1.0.92
- **Documentation** : Ajout d'une section de dépannage pour les erreurs I2C (`i2cRead returned Error -1`) et les redémarrages inattendus, en soulignant les causes matérielles probables (câblage, alimentation).

## Version 1.0.91
- **UI**: Inversion du sens de rotation de l'encodeur pour la navigation dans le menu pour une expérience plus intuitive (sens horaire = monter).

## Version 1.0.90
- **Correctif (Crash SD)** : Réécriture complète de la fonction de formatage pour être autonome et robuste, en gérant manuellement l'initialisation du bus SPI et l'allocation mémoire pour éviter les crashs de type "Guru Meditation Error".

## Version 1.0.89
- **Correctif (Crash SD)** : Utilisation des fonctions bas niveau `sdcard_init` pour le formatage, évitant le crash dû à l'échec du montage via `SD.begin`.

## Version 1.0.88
- **Correctif (Crash)** : Résolution du crash "Guru Meditation Error" lors du formatage de la carte SD en forçant la réinitialisation du matériel avant l'opération.
- **Correctif (UI)** : Simplification et fiabilisation de la logique de l'encodeur rotatif pour une navigation cohérente dans les menus et les pages sur OLED et LCD.

## Version 1.0.87
- **Correctif (Compilation)** : Correction définitive de l'erreur `getHeight` dans `ui_manager.cpp` en remplaçant l'appel de méthode par une constante locale définie selon l'environnement (OLED/LCD).

## Version 1.0.86
- **Correctif (Compilation)** : Résolution de l'erreur de compilation `getHeight is not a member of DisplayInterface` en utilisant des constantes de hauteur d'écran dépendant de l'environnement.

## Version 1.0.85
- **Correctif (UI)** : Correction du processus de formatage de la carte SD qui ne redémarre plus l'appareil et affiche un message de succès ou d'échec.
- **Fonctionnalité (UI)** : Le menu système est désormais scrollable sur l'écran OLED, rendant l'option "Format SD" accessible.
- **Correctif (UI)** : Activation de la fonction "Clear Logs" dans le menu système.

## Version 1.0.84
- **Correctif (Compilation)** : Résolution d'erreurs de compilation multiples dues à l'absence de la définition `BUTTON_GUARD_MS` et à l'inclusion manquante des en-têtes de pages (`pages_sh1106.h`, `pages_st7789.h`) dans `main.cpp` et `ui_manager.cpp`.

## Version 1.0.83
- **Correctif (Compilation)** : Correction du chemin d'inclusion de `display_interface.h` dans `ui_manager.h` pour résoudre l'erreur de compilation "No such file or directory".

## Version 1.0.82
- **Fonctionnalité (Maintenance)** : Ajout d'une option "Format SD" dans le menu système pour formater la carte SD (en FAT32) directement depuis l'appareil. Une étape de confirmation a été ajoutée pour prévenir les formatages accidentels.

## Version 1.0.81
- **Amélioration (Logs)** : Message d'erreur plus explicite en cas d'échec de montage de la carte SD (suggère le formatage FAT32).

## Version 1.0.80
- **Correctif (Compilation)** : Résolution des erreurs de compilation suite à la refonte de l'historique (méthodes manquantes `clearHistory`, `getRecentStats`, `LittleFS.mounted`).
- **Refactorisation** : Standardisation de la structure `Stats24h` pour l'API Web.

## Version 1.0.79
- **Fonctionnalité (Historique)** : Sauvegarde redondante des données sur carte SD (si présente) au format CSV (`/history_log.csv`), en plus de la mémoire interne.
- **Stabilité (SD)** : Amélioration de l'initialisation SD avec tentative de reconnexion à fréquence réduite (4MHz) en cas d'échec.

## Version 1.0.78
- **Fonctionnalité (Matériel)** : Ajout du support initial pour carte SD via `SdManager`.
- **Configuration** : Utilisation des définitions `SD_CS_PIN`, `SPI_SCK_PIN`, `SPI_MISO_PIN`, `SPI_MOSI_PIN` depuis `board_config.h`.

## Version 1.0.77
- **Documentation** : Ajout d'explications sur la corruption du système de fichiers (causes, prévention) et les alternatives de stockage (carte SD, Cloud) dans la FAQ et le guide de maintenance.

## Version 1.0.76
- **Correctif (Système)** : Ajout d'une procédure de récupération du système de fichiers (LittleFS). Maintenir le bouton BOOT au démarrage permet désormais de formater la mémoire en cas de corruption.
- **Stabilité** : Initialisation explicite de LittleFS et création du dossier `/history` au démarrage pour prévenir les erreurs d'écriture et les boucles de redémarrage (WDT).

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
