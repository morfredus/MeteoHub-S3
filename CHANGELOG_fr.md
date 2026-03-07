# [1.0.180] – 2026-03-07
- Ajout d'un fallback local `SD_DET_ACTIVE_LEVEL` dans `sd_manager.cpp` pour éviter toute erreur de symbole non défini selon l'ordre d'includes/toolchain.
- Maintien de `isCardDetected()` déclaré+défini dans `SdManager` avec check non bloquant au boot.

# [1.0.179] – 2026-03-07
- Réintroduction explicite de `isCardDetected()` dans `SdManager` pour lever l'erreur "identificateur non défini" signalée à la compilation/IDE.
- Vérification DET conservée non bloquante au démarrage SD (log diagnostic sans empêcher les tentatives de montage).

# [1.0.178] – 2026-03-07
- Correction supplémentaire de `SdManager::verifyWriteAccess()` pour supprimer définitivement les erreurs de parsing C++ (bloc unique, retours explicites, suppression fichier test centralisée).
- Réordonnancement des includes dans `sd_manager.cpp` (`Arduino.h` avant les logs) pour éviter les effets de bord de macro selon toolchain.

# [1.0.177] – 2026-03-07
- Correction de compilation dans `SdManager::verifyWriteAccess()` avec réécriture plus explicite de la séquence d'ouverture/écriture/fermeture du fichier test SD.
- Ajout de `#include <Arduino.h>` dans `sd_manager.cpp` pour fiabiliser la compilation croisée des types Arduino (`size_t`, API runtime) selon toolchain.

# [1.0.176] – 2026-03-07
- Réécriture complète du `SdManager` sur la méthode validée “mode stable 10MHz” : instance `FSPI` dédiée recréée avant chaque montage et `SD.begin(..., format_if_fail=...)`.
- Suppression de la dépendance bloquante à la broche DET dans la logique de montage pour éviter les faux négatifs de détection.
- Formatage robuste aligné sur le code de référence: remount à 10MHz avec `format_if_fail=true` puis test d'écriture critique.
- Conservation des fonctionnalités projet liées à la SD (historique `/history`, sauvegarde, lecture, upload/suppression via APIs existantes).

# [1.0.175] – 2026-03-07
- Renforcement du montage SD sur ESP32-S3: ajout d’essais à 1MHz et 400kHz en plus des fréquences rapides.
- Préparation explicite du bus SPI avant `SD.begin` (CS HIGH, MISO pull-up, clocks d'amorçage) pour améliorer la compatibilité des cartes/modules sensibles.
- Ajustement `max_files` lors du montage SD à 10 pour limiter les échecs liés aux ouvertures de fichiers simultanées.

# [1.0.174] – 2026-03-07
- Correction SD_DET: la détection de carte n'est plus bloquante pour le montage (certains modules ont une polarité inversée ou un signal bruité).
- Ajout d'un échantillonnage multi-lectures de la broche DET avec logs détaillés (LOW/HIGH) pour diagnostiquer le câblage réel.
- Ajout du paramètre `SD_DET_ACTIVE_LEVEL` (LOW/HIGH) dans `board_config.h` pour s'adapter aux lecteurs à polarité inversée.
- Si la carte est déjà montée, un état DET incohérent n'entraîne plus de démontage forcé; seule la vérification `SD.cardType()` décide de la disponibilité.

# [1.0.173] – 2026-03-07
- Refonte du gestionnaire SD pour s’aligner sur la méthode validée (SPI FSPI dédié + `SD.begin(..., format_if_fail=true)`).
- Respect strict du mapping défini dans `board_config.h` (CLK=9, D0/MISO=10, CMD/MOSI=11, D3/CS=12, DET=14).
- Ajout d’une détection de présence carte via `SD_DET_PIN` (LOW=présente) avant montage/réessais.
- Conservation des fonctionnalités SD existantes: lecture/écriture, incrémentation quotidienne des fichiers CSV d’historique, suppression/upload web et formatage.

# [1.0.172] – 2026-02-25
- Ajout et liens croisés de la documentation débutant (EN/FR) dans tous les documents utilisateur.
- Tous les guides, FAQ, configuration et index référencent désormais l'onboarding débutant.
- Version minimale valide : 1.0.172

# [1.0.171] – 2026-02-25
- Ajout de la gestion avancée des échelles pour les graphiques température, humidité et pression.
- Trois modes disponibles : fixe, dynamique, mixte (avec élargissement configurable).
- Contrôles interactifs sur l'UI web pour choisir le mode et le pourcentage.
- Aide contextuelle sous le graphique.
- Synchronisation automatique entre config.h et l'UI web.

# [1.0.170] - 2026-02-24
### Corrigé
- Application du même schéma de zone sûre que les graphes aux autres pages OLED.
- Conservation des titres dans la bande haute et déplacement du début de contenu prévisions/logs sous la zone haute réservée SSD1306.
- Ajustement de l’espacement des lignes de logs pour éviter le chevauchement haut sur les SSD1306 à bande jaune.
