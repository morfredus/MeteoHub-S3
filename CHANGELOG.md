
# [1.0.181] – 2026-03-07
- Hardened SD read-only handling: `SdManager::begin()` and `ensureMounted()` no longer report SD as available if `/history` creation or write test fails.
- `ensureHistoryDirectory()` now returns a boolean and attempts a fallback `/sd/history` for atypical mountpoints.
- On write failure after format/remount, SD is unmounted and marked unavailable to avoid repeated errors during history save.

# [1.0.180] – 2026-03-07
- Added local fallback `SD_DET_ACTIVE_LEVEL` in `sd_manager.cpp` to avoid undefined symbol errors depending on include/toolchain order.
- Maintained `isCardDetected()` declared+defined in `SdManager` with non-blocking check at boot.

# [1.0.179] – 2026-03-07
- Explicit reintroduction of `isCardDetected()` in `SdManager` to fix "undefined identifier" compilation/IDE error.
- DET check remains non-blocking at SD startup (diagnostic log, does not prevent mount attempts).

# [1.0.178] – 2026-03-07
- Further fix to `SdManager::verifyWriteAccess()` to permanently remove C++ parsing errors (single block, explicit returns, centralized test file removal).
- Reordered includes in `sd_manager.cpp` (`Arduino.h` before logs) to avoid macro side effects depending on toolchain.

# [1.0.177] – 2026-03-07
- Build fix in `SdManager::verifyWriteAccess()` with more explicit open/write/close test sequence.
- Added `#include <Arduino.h>` in `sd_manager.cpp` for reliable cross-compilation of Arduino types (`size_t`, runtime API) per toolchain.

# [1.0.176] – 2026-03-07
- Complete rewrite of `SdManager` using the validated “stable 10MHz mode”: dedicated FSPI instance recreated before each mount and `SD.begin(..., format_if_fail=...)`.
- Removed blocking dependency on DET pin in mount logic to avoid false negatives.
- Robust formatting aligned with reference code: remount at 10MHz with `format_if_fail=true` then critical write test.
- All SD-related project features preserved (history `/history`, save, read, upload/delete via existing APIs).

# [1.0.175] – 2026-03-07
- Strengthened SD mount on ESP32-S3: added 1MHz and 400kHz retries in addition to fast frequencies.
- Explicit SPI bus preparation before `SD.begin` (CS HIGH, MISO pull-up, clock priming) for better compatibility with sensitive cards/modules.
- Adjusted `max_files` during SD mount to 10 to limit failures due to simultaneous file opens.

# [1.0.174] – 2026-03-07
- SD_DET fix: card detection is no longer blocking for mount (some modules have inverted polarity or noisy signal).
- Added multi-read sampling of DET pin with detailed logs (LOW/HIGH) to diagnose actual wiring.
- Added `SD_DET_ACTIVE_LEVEL` parameter (LOW/HIGH) in `board_config.h` to adapt to inverted-polarity readers.
- If card is already mounted, inconsistent DET state no longer forces unmount; only `SD.cardType()` decides availability.

# [1.0.173] – 2026-03-07
- SD manager overhaul to align with validated method (dedicated FSPI SPI + `SD.begin(..., format_if_fail=true)`).
- Strict respect of mapping defined in `board_config.h` (CLK=9, D0/MISO=10, CMD/MOSI=11, D3/CS=12, DET=14).
- Added card presence detection via `SD_DET_PIN` (LOW=present) before mount/retries.
- All existing SD features preserved: read/write, daily CSV history increment, web upload/delete/formatting.

# [1.0.172] – 2026-02-25
- Added and cross-linked beginner documentation (EN/FR) in all user-facing docs.
- All guides, FAQ, configuration, and index now reference beginner onboarding.
- Minimum valid version: 1.0.172

# [1.0.171] – 2026-02-25
- Added advanced scale management for temperature, humidity and pressure graphs.
- Three modes: fixed, dynamic, mixed (with configurable expansion).
- Interactive controls on web UI to select mode and percentage.
- Contextual help below the graph.
- Automatic synchronization between config.h and web UI.

# [1.0.170] - 2026-02-24
### Fixed
- Applied the graph safe-zone layout scheme to other OLED pages.
- Kept page titles in the top band and moved forecast/logs content start below SSD1306 reserved top area.
- Aligned log line spacing to avoid top-content overlap in yellow-band SSD1306 variants.
