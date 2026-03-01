# Guide utilisateur

**Débutant ?** Voir le [Guide Débutant](../beginner/index_fr.md) ou [Beginner's Guide](../beginner/index.md)

Version minimale valide : 1.0.173

- Tourner l’encodeur pour naviguer entre les pages.
- Le clic encodeur ouvre le menu.
- Back sort du menu/des confirmations.
- Confirm valide les actions contextuelles.

Pages principales :
- Météo
- Prévisions
- Graphes (température, humidité, pression)
- Réseau
- Système
- Logs


Stratégie de stockage :
- LittleFS est utilisé comme cache local glissant (24-72h), et non comme archive infinie.
- Utiliser un collecteur externe qui interroge `/api/history?window=...&interval=...` toutes les X minutes.
- Stocker l'historique long terme dans InfluxDB (recommandé), puis visualiser avec Grafana ou Home Assistant.
