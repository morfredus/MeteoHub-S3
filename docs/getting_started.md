# Getting Started

**Beginner?** See [Beginner's Guide](../beginner/index.md) or [Guide Débutant](../beginner/index_fr.md)

Minimum valid version: 1.0.170

1. Clone the project.
2. Create `include/secrets.h` from `include/secrets_example.h`.
3. Select PlatformIO environment: `esp32-s3-oled` (ESP32-S3 DevKitC-1), `esp32-dev-oled` (ESP32 Dev Module), or `esp32-dev-oled-ultra-safe` (ESP32 Dev Module SD debug), or `esp32-dev-oled-hspi-debug` (ESP32 Dev Module HSPI debug).
4. Build with `platformio run`.
5. Flash with `platformio run --target upload`.
6. Open serial monitor (`platformio device monitor`) to verify boot.
