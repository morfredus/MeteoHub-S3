# [1.0.174] – 2026-03-07
- Réécriture de SdManager pour utiliser SPIClass(FSPI) à 40 MHz avec format_if_fail=true, identique à l'implémentation de référence validée.
- Suppression de la boucle multi-fréquences, des appels bas niveau sdcard_init/sdcard_uninit FatFS et de la configuration manuelle GPIO drive-strength.
- Remplacement du pointeur brut SPIClass par std::unique_ptr<SPIClass> membre dans SdManager.
- Simplification de format() via SD.begin avec format_if_fail=true au lieu de f_mkfs bas niveau.
- Version minimale valide : 1.0.174

# [1.0.173] – 2026-03-07
- Mise à jour du brochage SPI de la carte SD pour correspondre au câblage physique : CLK=9, MISO=10, MOSI=11, CS=12, DET=14.
- Ajout du support de SD_DET_PIN dans SdManager : détection physique de la présence de carte via entrée pull-up (LOW = carte insérée).
- SdManager saute les tentatives de montage en l'absence physique de carte, et marque immédiatement indisponible en cas de retrait à chaud.
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
