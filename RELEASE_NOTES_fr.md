# Release v1.0.181 – 2026-03-08

## Nouveautés et corrections majeures

- **Durcissement SD read-only** :
  - `SdManager::begin()` et `ensureMounted()` n’annoncent plus la SD disponible si la création de `/history` ou le test d’écriture échouent.
  - `ensureHistoryDirectory()` retourne désormais un booléen et tente un fallback `/sd/history` pour les cas de points de montage atypiques.
  - En cas d’échec d’écriture après format/remount, la SD est démontée et marquée indisponible pour éviter les erreurs répétées côté sauvegarde historique.
- **Gestion avancée de la détection SD** :
  - Ajout d’un fallback local `SD_DET_ACTIVE_LEVEL` dans `sd_manager.cpp` pour éviter toute erreur de symbole non défini selon l’ordre d’includes/toolchain.
  - Maintien de `isCardDetected()` déclaré+défini dans `SdManager` avec check non bloquant au boot.
  - Correction de compilation et robustesse accrue sur la séquence d’ouverture/écriture/fermeture du fichier test SD.
- **Refonte du gestionnaire SD** :
  - Instance FSPI dédiée recréée avant chaque montage, `SD.begin(..., format_if_fail=...)` systématique.
  - Suppression de la dépendance bloquante à la broche DET, gestion de la polarité configurable.
  - Multi-essais de montage à différentes fréquences (10MHz, 1MHz, 400kHz).
  - Ajustement du nombre de fichiers ouverts simultanément.
- **Synchronisation documentation** :
  - Tous les documents utilisateur (FR/EN) sont synchronisés et reflètent la version 1.0.181.
  - Ajout d’un tableau de mapping GPIO détaillé dans la documentation matérielle.
  - Ajout d’une section “Fonctionnalités avancées et API web” dans les guides utilisateur (FR/EN).
  - CHANGELOG.md et CHANGELOG_fr.md sont strictement équivalents.

## Fonctionnalités avancées documentées
- API web complète (données live, historique, stats, logs, fichiers, OTA, diagnostic système).
- Gestion avancée des fichiers (LittleFS/SD) via l’interface web.
- Processus OTA web documenté.
- Diagnostic système et logs accessibles.
- Mode maintenance (formatage LittleFS par BOOT).
- Gestion des alertes météo et tendances (traduction, synthèse, couleurs).

---

**Version minimale requise dans tous les documents : 1.0.181**

Pour plus de détails, voir le CHANGELOG et la documentation utilisateur.