# Démarrage rapide

**Débutant ?** Voir le [Guide Débutant](../beginner/index_fr.md) ou [Beginner's Guide](../beginner/index.md)

Version minimale valide : 1.0.170

1. Cloner le projet.
2. Créer `include/secrets.h` à partir de `include/secrets_example.h`.
3. Sélectionner l’environnement PlatformIO : `esp32-s3-oled` (ESP32-S3 DevKitC-1) ou `esp32-dev-oled` (ESP32 Dev Module).
4. Compiler avec `platformio run`.
5. Flasher avec `platformio run --target upload`.
6. Ouvrir le moniteur série (`platformio device monitor`) pour valider le démarrage.
