
# Journal des modifications du projet

Version minimale valide : 1.0.144







## Version 1.0.144
- **Changement (Stratégie OLED)** : Suppression de la logique d'auto-détection/bascule hot-plug OLED et passage à une sélection explicite du contrôleur dans `config.h` (`OLED_CONTROLLER`).
- **Simplification (Driver OLED)** : Le wrapper OLED initialise uniquement le contrôleur configuré (SH1106 ou SSD1306), réduisant les ambiguïtés runtime.

## Version 1.0.143
- **Correctif (Bascule OLED AUTO Runtime)** : En reconnexion OLED, le mode AUTO retente d'abord l'initialisation avec le driver opposé puis fallback, pour gérer les vrais hot-swap SH1106/SSD1306.
- **Stabilité (Récupération Hot-Plug)** : Ajout d'une stratégie dédiée de réinitialisation en reconnexion au lieu de réutiliser systématiquement le driver précédent.

## Version 1.0.142
- **Correctif (Neige SH1106 en Hot-Plug)** : Durcissement de la séquence de réinitialisation/nettoyage SH1106 et ajout d'une récupération runtime pour éviter les pixels bruités après débranchement/rebranchement.
- **Stabilité (OLED Runtime)** : Ajout de vérifications de présence I2C lors du flush d'affichage et réinitialisation automatique du driver quand le module OLED revient.

## Version 1.0.141
- **Correctif (Bascule OLED AUTO)** : Le mode AUTO privilégie désormais l’initialisation SSD1306 puis fallback SH1106 pour améliorer la compatibilité lors du remplacement d’un SH1106 par un SSD1306.
- **Correctif (Pixels fantômes SSD1306)** : Durcissement de la séquence d’initialisation/nettoyage SSD1306 (`resetDisplay` + clear/display supplémentaire) pour réduire les pixels résiduels après changement de module.

## Version 1.0.140
- **Fonctionnalité (Compatibilité OLED)** : Ajout de la prise en charge double driver OLED (SH1106/SSD1306) dans l’environnement OLED avec détection automatique d’adresse I2C et mode driver configurable.
- **Amélioration (Abstraction Affichage)** : Le wrapper OLED utilise désormais un backend générique `OLEDDisplay` et une initialisation runtime pour améliorer la compatibilité entre modules OLED.

## Version 1.0.139
- **Correctif (Régression Rendu OLED)** : Suppression du `show()` intermédiaire forcé lors des transitions de contexte UI pour éviter le clignotement d’image vide et la dégradation d’affichage OLED.
- **Stabilité (Nettoyage OLED)** : Conservation du clear de buffer au changement de contexte, avec flush uniquement via le chemin normal de rendu de page.

## Version 1.0.138
- **Correctif (Artefacts Changement de Page OLED)** : Ajout d’un clear+flush forcé lors des changements de contexte d’écran (page/menu/confirmation) pour éviter les pixels résiduels de la page précédente.
- **Stabilité UI** : Ajout d’un suivi d’état de rendu dans `UiManager` pour détecter les transitions et déclencher un nettoyage plein écran déterministe.

## Version 1.0.137
- **Correctif (Robustesse SD)** : Renforcement des contrôles de disponibilité SD avec tentative automatique de remontage et logique de reconnexion temporisée quand la carte devient indisponible.
- **Correctif (Montage SD)** : Unification des retries de montage dans `SdManager` et création systématique du dossier `/history` après remontage pour réduire les erreurs "SD absente/erreur".

## Version 1.0.136
- **UI (Taille Cartouche Alerte)** : Réduction de 25% de la hauteur du cartouche alerte pour compacter le dashboard tout en conservant l’accès au détail.
- **UI (Gestion Dépassement)** : Ajout d’un défilement vertical pour les textes d’alerte/modale trop longs et garantie de visibilité de l’icône parchemin en bas à droite.

## Version 1.0.135
- **Correctif (UI Tendance Statistiques)** : La page Statistiques affiche désormais les lignes de tendance depuis `/api/stats` et ne reste plus sur "Chargement...".
- **Fonctionnalité (Tendance Météo Globale)** : Ajout de `trend.global_label_fr` dans `/api/stats` avec un libellé synthétique (ex. "Vers beau temps", "Vers pluie").

## Version 1.0.134
- **Correctif (Langue Alerte Courte)** : Ajout d’un fallback pour les intitulés d’événements anglais non reconnus afin que l’alerte courte Web reste en français.
- **UI (Déclencheur Détails Alerte)** : Remplacement du gros bouton "Voir détails complets" par une petite icône parchemin ancrée en bas à droite du cartouche.

## Version 1.0.133
- **Correctif (Langue Détail Alerte)** : Le texte détaillé d’alerte renvoyé par les APIs Web est désormais un résumé déterministe en français, sans reprise de texte brut anglais du fournisseur.
- **Clarification (Comportement API)** : `/api/alert` et `/api/live` continuent d’exposer les champs FR, tout en conservant le paramètre `lang` côté requête prévision.

## Version 1.0.132
- **Optimisation (Rafraîchissement Alerte)** : Le polling d’alerte Web passe à 15 minutes (`ALERT_REFRESH_MS`) au lieu de 5 secondes pour réduire les requêtes inutiles.
- **Mise à jour de comportement (Dashboard)** : Le cartouche d’alerte n’est plus rafraîchi via `/api/live` ; il est mis à jour uniquement via la planification dédiée de `/api/alert`.

## Version 1.0.131
- **Correctif (Langue Détails Alerte)** : Les détails d’alerte Web utilisent désormais uniquement les champs français et appliquent un résumé de secours en français quand le texte fournisseur n’est pas traduisible de façon fiable.
- **Correctif (Stabilité Dashboard)** : Le cartouche d’alerte a une hauteur fixe et des zones de texte contraintes pour supprimer l’effet d’ascenseur de la page lors des rafraîchissements.

## Version 1.0.130
- **Correctif (Langue Alerte Web)** : Ajout d'un champ de description d'alerte en français (`description_fr`) dans `/api/alert` et `alert_description_fr` dans `/api/live`.
- **Correctif (Modal Détails Alerte)** : Le dashboard priorise désormais la description française pour le cartouche et la modal de détails.

## Version 1.0.129
- **Fonctionnalité (Contexte Alerte Web)** : Ajout des champs de validité (`start_unix`, `end_unix`) et du champ prêt à l’emploi `alert_level_label_fr` dans les APIs Web pour exposer directement le niveau en français.
- **Fonctionnalité (UX Alerte Web)** : Le cartouche d’alerte affiche maintenant la période de validité et un bouton "Voir détails complets" ouvrant une modal lisible (description longue + consigne de sécurité).
- **Fonctionnalité (Fiabilité Live)** : Ajout d’un badge visuel "Capteur invalide" sur le dashboard quand `sensor_valid=false` pour éviter les mauvaises interprétations des cartouches live.

## Version 1.0.128
- **Correctif (Alertes Web)** : Ajout d'un endpoint dédié `/api/alert` et utilisation systématique de la traduction française (`event_fr`) pour aligner l'UI Web avec l'OLED.
- **Amélioration (UX Alerte Web)** : Le cartouche d'alerte du dashboard adapte maintenant sa couleur de fond et de texte selon la sévérité, avec contraste lisible, et affiche des détails plus complets (source + description).
- **Correctif (Valeurs capteurs live)** : `/api/live` renvoie désormais les vraies mesures issues de `SensorManager` (`temp`, `hum`, `pres`) au lieu de valeurs fictives, pour afficher les données réelles dans les cartouches.
- **Intégration** : `WebManager` reçoit maintenant `SensorManager` au démarrage pour alimenter les APIs live.

## Version 1.0.127
- **Correctif (Langue Alerte Web)** : L'API Web `/api/live` expose désormais les champs d'alerte météo depuis `ForecastManager`, avec un libellé traduit en français (`alert_event_fr`) pour aligner le rendu Web avec l'OLED.
- **Fonctionnalité (Dashboard Web)** : Ajout d'une carte d'alerte météo sur la page principale, alimentée en temps réel par `data/app.js` (événement, niveau, émetteur).
- **Correctif (Intégration)** : `WebManager` reçoit désormais une référence à `ForecastManager` au démarrage pour rendre les alertes disponibles côté endpoints Web.

## Version 1.0.126
- **Correctif (Cohérence Documentation)** : Suppression des marqueurs de fusion non résolus et resynchronisation complète des documents FAQ/dépannage (`docs/*.md` et `docs/*_fr.md`).
- **Optimisation (Chargement Historique)** : Optimisation du tronquage d'historique surdimensionné dans `HistoryManager::loadRecent()` via une suppression en plage unique au lieu de suppressions successives en tête.

## Version 1.0.125
- **Correctif (Intégrité CSV Historique)** : Correction d'un comportement indéfini lors de l'écriture du timestamp dans le CSV SD en utilisant un format compatible 64 bits (`%lld`) avec validation stricte de la taille du buffer avant écriture. Cela évite l'insertion de fragments binaires corrompus dans les fichiers CSV.
- **Amélioration (Écriture SD Historique)** : Utilisation de `File.write()` avec un nombre exact d'octets pour chaque ligne formatée afin de fiabiliser l'écriture.

## Version 1.0.124
- **Correctif (Footer Web)** : Suppression du nom/version codés en dur dans `data/footer.js` ; le footer lit désormais `project_name` et `project_version` depuis `/api/system` (valeurs issues des flags PlatformIO).
- **Correctif (Web UI)** : Mise à jour de `data/app.js` pour lire `project_version` depuis `/api/system` avec compatibilité descendante.

## Version 1.0.123
- **Correctif (Build)** : Correction des littéraux JSON échappés dans `/api/history` (`web_manager.cpp`) après la mise à jour d'agrégation par intervalle, rétablissant la compilation C++.

## Version 1.0.122
- **Correctif (Graphiques Web)** : `/api/history` prend désormais en charge une agrégation explicite par `interval` avec moyenne côté serveur, afin de réduire la taille des réponses et la charge CPU tout en conservant des courbes fluides.
- **Fonctionnalité (Graphe Dashboard)** : Le tableau de bord demande exactement les 2 dernières heures avec un pas de 5 minutes (`window=7200`, `interval=300`).
- **Fonctionnalité (Graphe Historique 24H)** : La page historique demande exactement les 24 dernières heures avec un pas de 30 minutes (`window=86400`, `interval=1800`) pour réduire l'usage mémoire et le temps de rendu.
- **Rendu UI** : Les datasets Chart.js utilisent explicitement des courbes monotones non "stepped" pour relier les points sans angles droits artificiels.

## Version 1.0.121
- **Fix (Build)** : Corrected JSON string escaping in `web_manager.cpp` for `/api/history` response generation, fixing C++ compilation errors in `esp32-s3-oled` and restoring firmware build.

## Version 1.0.120
- **Correctif (Web Performance)** : Refactor de `data/app.js` pour charger uniquement les données nécessaires selon la page (historique seulement sur Dashboard/Long-term, statistiques seulement sur la page Stats), ce qui supprime les appels API lourds inutiles et réduit les blocages UI.
- **Correctif (API History)** : Optimisation de `/api/history` avec paramètres `window` et `points` pour limiter côté serveur la fenêtre temporelle et le nombre de points renvoyés, réduisant fortement le temps de réponse et la charge CPU.
- **Amélioration (Web UX)** : Réduction de la fréquence de rafraîchissement des gros endpoints (`history`, `stats`) pour fluidifier l'interface et éviter les pics de charge.

## Version 1.0.119
- **Correctif (SD)** : Renforcement du formatage SD avec plusieurs tentatives à vitesses SPI décroissantes (4MHz, 1MHz, 400kHz), réinitialisation bas niveau entre chaque essai, et remount/validation automatique après formatage.
- **Amélioration (SD)** : Renforcement du montage SD au démarrage avec retries multi-fréquences (8MHz, 4MHz, 1MHz) pour mieux tolérer les cartes instables et les montages sensibles.

## Version 1.0.118
- **Correctif (Web UI)** : Ajout de la route serveur `/menu.js` dans `web_manager.cpp` pour servir le nouveau menu partagé intégré au firmware. Le menu apparaît désormais correctement après compilation et rechargement navigateur.

## Version 1.0.117
- 1) **Web UI (Menu partagé)** : Ajout d'un menu de navigation commun injecté par `data/menu.js` et utilisé sur toutes les pages web pour garantir une navigation identique et facile à maintenir.
- 2) **Web UI (Page principale)** : Le graphique du tableau de bord affiche désormais uniquement les 2 dernières heures, avec mise à jour automatique continue et rendu plus fluide (courbes monotones sans angles marqués).
- 3) **Web UI (Historique long terme)** : La page d'historique long terme affiche désormais uniquement les 24 dernières heures.
- 4) **Web UI (Démarrage & rechargement)** : À l'ouverture de l'interface, les données historiques sont immédiatement rechargées depuis l'API, puis rafraîchies périodiquement afin d'afficher l'historique restauré après démarrage selon les nouvelles fenêtres temporelles (2h / 24h).

## Version 1.0.116
- **Correctif (Serveur Web)** : Implémentation du sous-échantillonnage (downsampling) côté serveur pour l'API de l'historique (`/api/history`). Cela réduit drastiquement la quantité de données envoyées pour les graphiques, corrigeant de manière définitive les redémarrages (watchdog) avec de grands volumes de données et améliorant la scalabilité.
- **Correctif (Documentation)** : Correction d'un texte corrompu et ajout d'informations manquantes dans la FAQ en français (`faq_fr.md`).

## Version 1.0.115
- **Documentation** : Mise à jour complète de tous les guides utilisateur (Guide, FAQ, Dépannage, Architecture) pour refléter les fonctionnalités et correctifs jusqu'à la v1.114, en assurant une cohérence bilingue totale.

## Version 1.0.114
- **Correctif (Serveur Web)** : Ajout d'un petit délai dans la boucle de streaming de l'endpoint `/api/history` pour rendre la main au système, prévenant ainsi de manière définitive les redémarrages ("Watchdog Timeout") lors du chargement de très grands volumes de données d'historique.

## Version 1.0.113
- **Correctif (Compilation)** : Correction d'une erreur de syntaxe dans `web_manager.cpp` (capture lambda) qui empêchait la compilation de l'endpoint `/api/system`.

## Version 1.0.112
- **Correctif (Build)** : Déplacement des fichiers utilitaires `system_info` vers le répertoire correct `src/utils/` pour résoudre les erreurs de compilation. Nettoyage des fichiers mal placés dans `docs/`.

## Version 1.0.111
- **Refactorisation**: Création d'un utilitaire `system_info` centralisé.
- **Fonctionnalité (API Web)**: L'endpoint `/api/system` fournit maintenant des informations système complètes (CPU, mémoire, systèmes de fichiers, réseau).

## Version 1.0.110
- **Correctif (Serveur Web)** : Optimisation de l'endpoint `/api/history` pour envoyer les données par blocs, prévenant ainsi les redémarrages ("Watchdog Timeout") lors du chargement des graphiques avec un grand nombre de points de données.

## Version 1.0.109
- **Correctif (Serveur Web)** : Correction d'un bug critique qui provoquait un redémarrage ("Watchdog Timeout") lors du téléchargement de fichiers volumineux (historique, logs) depuis l'interface web. Le gestionnaire de téléchargement utilise désormais une méthode de streaming non-bloquante au lieu de charger le fichier entier en mémoire.

## Version 1.0.108
- **Documentation** : Amélioration du guide de dépannage pour les crashs "Watchdog Timeout", en clarifiant que la solution définitive est d'utiliser le streaming de fichiers dans le code du serveur web pour éviter les opérations bloquantes.

## Version 1.0.107
- **Documentation** : Synchronisation de la documentation anglaise (`maintenance_and_troubleshooting.md`, `faq.md`) avec la version française pour corriger la numérotation, ajouter des sections manquantes et mettre à jour les informations obsolètes sur le chargement des graphiques.

## Version 1.0.106
- **Documentation** : Ajout d'une entrée de dépannage spécifique pour les crashs de l'interface Web (Watchdog Timeout) causés par une communication instable avec la carte SD (erreurs CRC).

## Version 1.0.105
- **Projet** : Ajout du fichier `.cursorrules` pour établir les règles globales de codage et le contexte du projet pour l'IA.

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
