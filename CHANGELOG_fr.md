# [1.0.178] – 2026-03-01
1. Mise à jour du mapping SD ESP32 Dev Module avec une broche CS plus sûre (`GPIO13`) pour éviter les instabilités liées au strap de `GPIO5`.
2. Ajout de diagnostics SD détaillés (type de carte, états des broches SPI, fréquence/index par tentative, logs d’initialisation SPI).
3. Ajout de l’environnement PlatformIO `esp32-dev-oled-ultra-safe` activant `SD_ULTRA_SAFE_DEBUG` (SPI très basse vitesse et drive strength conservateur) pour le diagnostic SD.
- Version minimale valide : 1.0.178

# [1.0.177] – 2026-03-01
1. Changement de la table de partitions `esp32-dev-oled` de `default.csv` vers `huge_app.csv` afin de fournir assez d’espace flash pour le binaire firmware.
2. Fonctionnalité projet inchangée ; seul le layout mémoire de build pour ESP32 Dev Module a été ajusté.
- Version minimale valide : 1.0.177

# [1.0.176] – 2026-03-01
1. Correction de l’échec de build `esp32-dev-oled` en activant les symboles OLED pour le chemin de compilation ESP32 Dev Module.
2. Unification des gardes de compilation OLED pour que `main`, `ui_manager` et les pages OLED soient compilés pour `esp32-s3-oled` et `esp32-dev-oled`.
3. Fonctionnalité runtime inchangée tout en rétablissant l’intention de compilation multi-environnements.
- Version minimale valide : 1.0.176

# [1.0.175] – 2026-03-01
1. Ajout d’un second environnement PlatformIO `esp32-dev-oled` pour les builds ESP32 Dev Module avec mapping de partition `default.csv` compatible flash.
2. Ajout d’un mapping de broches sûr pour ESP32 Dev Module dans `include/board_config.h` en conservant le mapping ESP32-S3 existant et le comportement runtime.
3. Mise à jour de la documentation EN/FR pour décrire les deux environnements carte et les étapes de sélection.
- Version minimale valide : 1.0.175

# [1.0.174] – 2026-02-27
- Renforcement de la gestion de la carte SD pour modules ESP32-S3 3.3V avec tentatives de montage SPI adaptatives et reprise améliorée.
- Ajout de vérifications périodiques d’état SD et d’un backoff de reconnexion plus sûr pour réduire les états instables.
- Amélioration des écritures CSV d’historique avec création contrôlée du dossier, flush et nouvelle tentative après remontage.
- Version minimale valide : 1.0.174

# [1.0.173] – 2026-02-26
- Ajout de l'API diagnostic OLED (FPS, durée de rendu, erreurs I2C) exposée dans `/api/system`.
- La page système de l'UI web affiche désormais les diagnostics OLED en temps réel.
- Version minimale valide : 1.0.173
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
