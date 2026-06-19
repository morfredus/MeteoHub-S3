# [1.1.3] – 2026-03-15
### Fixed
- **Menu display corruption on item selection**: Added `d->clear()` at the start of the menu rendering block in `UiManager::drawPage()`. When navigating menu items, the screen was not cleared before redrawing because `screen_context_changed` was `false` (menu mode had not changed). Old items were drawn on top of new ones, causing a corrupted display.

# [1.1.2] – 2026-03-08
### Fixed
Critical file system corruption fix and related compilation errors.

1. **Explicit `flush()` calls before file close**
   - **Issue**: Data remained in RAM cache and was not physically written to SD/LittleFS before closing, causing FAT table corruption during rapid writes or power loss.
   - **Fix**: Systematic addition of `file.flush()` before every `file.close()` in `history_manager.cpp` (`saveRecent`, `saveToSd`) and `sd_manager.cpp` (`verifyWriteAccess`).
   - **Impact**: Ensures integrity of CSV and binary files after every write operation.

2. **Fixed `flush()` return type handling**
   - **Issue**: Compilation error "expression must have bool type" because `file.flush()` returns `void` on some ESP32 core versions, but code attempted to evaluate it in an `if`.
   - **Fix**: Removed conditional checks `if (!file.flush())`. Function is now called imperatively.
   - **Files**: `src/managers/sd_manager.cpp`, `src/managers/history_manager.cpp`.

3. **Added missing `cooperative_yield.h` include**
   - **Issue**: Compilation error "identifier undefined" for macro `COOPERATIVE_YIELD_EVERY` in `history_manager.cpp`.
   - **Fix**: Added `#include "../utils/cooperative_yield.h"` at the top of the file.

4. **Implemented Mutex for write protection**
   - **Issue**: Risk of corruption if two tasks (e.g., history save and web test) write to SD simultaneously.
   - **Fix**: Added `std::mutex` in `SdManager` and used `std::lock_guard` in critical methods (`verifyWriteAccess`, `ensureHistoryDirectory`, `openFileSafe`).

5. **Simplified SD mount logic**
   - **Issue**: Mount failures at 10MHz and 4MHz on sensitive cards.
   - **Fix**: Single frequency fixed at 1 MHz (1000000 Hz) for maximum stability, removing unnecessary multi-frequency retries.

# [1.1.1] – 2026-03-08
- Fixed compilation error in `SdManager::resetSpiBus()`: `SPIClass::begin()` returns `void` on ESP32, removed boolean capture.
- Simplified SD mount logic: single attempt at 1 MHz to maximize stability.
- Increased SPI initialization delays to ensure electrical stability during mount.

# [1.1.0] – 2026-03-08
- Complete `WebManager` refactor to exclusively use `std::string` (C++ Standard).
- Explicit conversion at boundaries between Arduino types (`String`) and C++ (`std::string`).
- Full support for file management (upload, download, delete) and OTA.

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
- Complete rewrite of `SdManager` using the validated "stable 10MHz mode": dedicated FSPI instance recreated before each mount and `SD.begin(..., format_if_fail=...)`.
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