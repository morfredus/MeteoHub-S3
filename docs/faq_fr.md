## Description alerte météo
L’API `/api/alert` retourne désormais le texte complet de l’alerte (en français si disponible). Le dashboard web affiche ce texte pour une clarté et une localisation maximale.
# FAQ

Version minimale valide : 1.0.127

## Nouvelles fonctionnalités (depuis 1.0.127)
- **Cartouche alerte météo** : Le dashboard affiche en permanence une cartouche d’alerte météo (via `/api/alert`).
- **Données capteurs en temps réel** : L’endpoint `/api/live` retourne les valeurs réelles des capteurs (température, humidité, pression).
- **Dashboard vs Historique** : Le graphique du dashboard affiche les 2 dernières heures ; la page historique affiche 24h.
- **Conformité stricte au projet** : Toutes les routes API sont déclarées uniquement dans `_setupApi()`. Aucun commentaire interdit ou placeholder n’existe dans le code.


## Internet est-il obligatoire ?
Oui, pour les mises à jour de prévisions et d’alertes. Les pages capteurs locales restent utilisables sans internet.

## Puis-je utiliser un seul capteur ?
Oui. Le projet gère la disponibilité partielle des capteurs avec des comportements de secours.

## Pourquoi les graphes sont vides après redémarrage ?
Ce comportement n’est plus normal. Le système recharge l’historique récent au démarrage, ce qui rend les graphes disponibles immédiatement sur OLED, LCD et Web UI.

## Quelle est la fréquence de mise à jour des prévisions ?
Environ toutes les 30 minutes.

## Comment accéder à l’interface Web ?
Connectez-vous au même réseau Wi-Fi puis ouvrez `http://meteohub.local`. Si cela ne fonctionne pas, utilisez l’adresse IP affichée sur la page Réseau.

## Quelle est la capacité de l’historique ?
- **Court terme (graphiques)** : environ 24h de données haute résolution (1 point/min) en RAM avec persistance.
- **Long terme** : quasi illimité (selon la taille de la carte SD) sous forme de CSV journaliers.

## Comment récupérer mes données ?
Dans l’interface Web, cliquez sur l’icône disquette 💾 du footer puis téléchargez les CSV dans `/history/YYYY-MM-DD.csv`.

## Puis-je changer la vitesse de rafraîchissement des pages ?
Oui, modifiez `DASHBOARD_REFRESH_MS` dans `include/config.h`.

## Comment changer la résolution LCD (240x240 vs 240x320) ?
Modifiez `LCD_WIDTH` et `LCD_HEIGHT` dans `include/config.h`. L’interface s’adapte automatiquement après redémarrage.

## Puis-je changer le contraste de l’OLED ?
Oui, modifiez `OLED_CONTRAST` dans `include/config.h`.

## Comment éviter la corruption des données ou la perte d’historique ?
La corruption du système de fichiers est rare mais possible après une coupure de courant brutale.
1. **Alimentation stable d’abord** : utilisez une alimentation/câble USB fiables.
2. **Récupération d’urgence (formatage LittleFS)** : en cas de boucle de redémarrage, débranchez, maintenez **BOOT**, rebranchez, attendez le message de maintenance, puis relâchez. L’appareil formate la mémoire interne puis redémarre (historique perdu).
3. **Robustesse maximale (avancé)** : stockez l’historique sur SD et/ou envoyez les données vers un service cloud (MQTT, ThingSpeak, etc.).

## Le projet gère-t-il OLED et LCD ?
Oui. Le firmware auto-détecte SH1106 OLED et ST7789 LCD au démarrage, sans changement de code.

## Quelles sont les différences principales OLED vs LCD ?
- **OLED (SH1106)** : monochrome, 128x64, navigation adaptée au layout OLED.
- **LCD (ST7789)** : couleur, 240x240/320, navigation/layout adaptés LCD.

## Où modifier le mapping GPIO ?
Dans `include/board_config.h`.

## Que faire si le téléversement échoue plusieurs fois ?
Essayez un autre câble USB, vérifiez le port série/COM, fermez le moniteur série, puis relancez le téléversement.
