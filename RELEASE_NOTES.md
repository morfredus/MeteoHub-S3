# Release v1.0.181 – 2026-03-08

## Major changes and fixes

- **SD read-only hardening**:
  - `SdManager::begin()` and `ensureMounted()` no longer report SD as available if `/history` creation or write test fails.
  - `ensureHistoryDirectory()` now returns a boolean and attempts a fallback `/sd/history` for atypical mountpoints.
  - On write failure after format/remount, SD is unmounted and marked unavailable to avoid repeated errors during history save.
- **Advanced SD detection management**:
  - Added local fallback `SD_DET_ACTIVE_LEVEL` in `sd_manager.cpp` to avoid undefined symbol errors depending on include/toolchain order.
  - Maintained `isCardDetected()` declared+defined in `SdManager` with non-blocking check at boot.
  - Build fix and increased robustness in SD test file open/write/close sequence.
- **SD manager overhaul**:
  - Dedicated FSPI instance recreated before each mount, systematic `SD.begin(..., format_if_fail=...)`.
  - Removed blocking dependency on DET pin, configurable polarity management.
  - Multi-frequency mount retries (10MHz, 1MHz, 400kHz).
  - Adjusted max simultaneous open files.
- **Documentation synchronization**:
  - All user-facing docs (EN/FR) are synchronized and reflect version 1.0.181.
  - Added detailed GPIO mapping table in hardware documentation.
  - Added “Advanced features and web API” section in user guides (EN/FR).
  - CHANGELOG.md and CHANGELOG_fr.md are strictly equivalent.

## Documented advanced features
- Full web API (live data, history, stats, logs, files, OTA, system diagnostics).
- Advanced file management (LittleFS/SD) via web UI.
- Documented web OTA process.
- System diagnostics and logs accessible.
- Maintenance mode (format LittleFS by BOOT button).
- Weather alert and trend management (translation, summary, colors).

---

**Minimum required version in all docs: 1.0.181**

For details, see the CHANGELOG and user documentation.