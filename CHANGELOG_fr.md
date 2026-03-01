# [1.0.174] – 2026-03-01
- Correction des freezes OLED et anomalies d'affichage lors de la rotation de l'encodeur en retirant la réécriture périodique LittleFS du chemin critique.
- Le cache historique conserve maintenant une fenêtre glissante allégée de 48h (`MAX_RECENT_RECORDS=2880`) pour réduire la charge runtime tout en gardant la résilience court terme.
- La compaction LittleFS est désormais effectuée uniquement au démarrage lorsqu'un fichier de cache surdimensionné est détecté.
- Version minimale valide : 1.0.174

# [1.0.173] – 2026-03-01
- Ajout d'un script de collecteur externe (`scripts/history_collector.py`) qui interroge `/api/history?window=...&interval=...` et pousse les données vers InfluxDB.
- Mise à jour de la politique de cache historique : LittleFS conservé comme cache glissant borné à 72h (pas une archive infinie).
- Ajout de la documentation EN/FR sur la collecte externe, la stratégie de rétention et l'intégration dashboard (Grafana/Home Assistant).
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
