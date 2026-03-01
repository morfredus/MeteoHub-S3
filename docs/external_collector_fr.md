# Collecteur externe d'historique

Version minimale valide : 1.0.173

## Objectif
Utiliser MeteoHub S3 comme cache edge court terme et pousser les séries temporelles vers un backend long terme.

- Conserver **LittleFS** comme cache local glissant uniquement (24 à 72 heures).
- Interroger `GET /api/history?window=...&interval=...` depuis un collecteur externe toutes les X minutes.
- Stocker les données côté serveur (recommandé : **InfluxDB**) et construire les tableaux de bord avec **Grafana** ou **Home Assistant**.

## Architecture recommandée
1. MeteoHub S3 expose `/api/history`.
2. Un collecteur Python (NAS/Raspberry Pi/serveur) interroge l'endpoint.
3. Le collecteur écrit les points dans InfluxDB.
4. Grafana/Home Assistant lit InfluxDB pour les vues long terme.

## Script de collecte
Un collecteur prêt à l'emploi est disponible dans `scripts/history_collector.py`.

Exemple d'exécution :

```bash
python3 scripts/history_collector.py \
  --meteo-url http://meteohub-s3.local \
  --window 3600 \
  --interval 60 \
  --period-min 5 \
  --influx-url http://127.0.0.1:8086 \
  --influx-org mon_org \
  --influx-bucket meteo \
  --influx-token <TOKEN>
```

## Stratégie de rétention
- **Appareil (LittleFS) :** cache glissant 24–72h pour la résilience et la reprise rapide après redémarrage.
- **Serveur (InfluxDB) :** rétention longue (mois/années).
- **Dashboards :** utiliser InfluxDB comme source de vérité pour l'historique analytique.
