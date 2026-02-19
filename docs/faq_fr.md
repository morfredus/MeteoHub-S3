# FAQ

Version minimale valide : 1.0.116


## Internet est-il obligatoire ?
Oui, pour les mises à jour de prévisions et d’alertes. Les pages capteurs locales restent utilisables sans internet.

## Puis-je utiliser un seul capteur ?
Oui. Le projet supporte la disponibilité partielle des capteurs et des comportements de secours.

## Pourquoi les graphes sont vides après redémarrage ?
Ce problème est résolu. Le système recharge désormais l'historique des dernières 24 heures au démarrage, rendant les graphiques immédiatement disponibles sur tous les écrans (OLED, LCD et Web).

## Quelle est la fréquence de mise à jour des prévisions ?
Environ toutes les 30 minutes.

## Comment accéder à l'interface Web ?
Connectez-vous au même réseau Wi-Fi et ouvrez `http://meteohub.local` dans votre navigateur. Si cela ne fonctionne pas, utilisez l'adresse IP affichée sur l'écran du MeteoHub (Page Réseau).

## Quelle est la capacité de l'historique ?
- **Court terme (Graphiques)** : ~24h de données haute résolution (1 point/min) stockées en RAM et sauvegardées.
- **Long terme** : Illimité (selon la taille de la carte SD) sous forme de fichiers CSV journaliers.

## Comment récupérer mes données ?
Allez sur l'interface Web, cliquez sur l'icône disquette 💾 en bas de page. Vous pouvez naviguer dans la carte SD et télécharger les fichiers CSV (`/history/YYYY-MM-DD.csv`).

## Puis-je changer la vitesse de rafraîchissement des pages ?
Oui, modifier `DASHBOARD_REFRESH_MS` dans `include/config.h`.


## Comment puis-je éviter la corruption des données ou la perte de l'historique ?
La corruption du système de fichiers est rare mais peut survenir lors d'une coupure de courant.
1.  **Prévention n°1 : Alimentation stable.** Utilisez une alimentation USB de bonne qualité et un câble fiable. C'est la cause la plus fréquente des problèmes.
2.  **Récupération facile (formatage d'urgence) :** Si l'appareil est bloqué ou redémarre en boucle, vous pouvez forcer un formatage d'urgence de la mémoire interne (LittleFS) sans ordinateur. Débranchez l'alimentation, maintenez le bouton **BOOT** enfoncé, rebranchez l'alimentation, attendez le message de maintenance à l'écran, puis relâchez le bouton. L'appareil effacera la mémoire et redémarrera proprement (toutes les données d'historique seront perdues). Cette procédure est accessible à tout utilisateur, même débutant.
3.  **Robustesse maximale (avancé) :** Pour les applications critiques, les alternatives les plus sûres sont de stocker les données sur une **carte SD** (nécessite une modification matérielle) ou de les envoyer vers un **service Cloud** (ex: ThingSpeak, broker MQTT), ce qui les met à l'abri de toute défaillance matérielle locale.

Le firmware utilise déjà des techniques d'écriture robustes (ajout de données plutôt que réécriture complète) pour minimiser l'usure et les risques.


## Comment changer la résolution de l'écran LCD (240x240 vs 240x320) ?
Modifiez les constantes `LCD_WIDTH` et `LCD_HEIGHT` dans `include/config.h`. L'interface s'adaptera automatiquement à la nouvelle résolution au prochain démarrage.


## Le projet gère-t-il les écrans OLED et LCD ?
Oui. Le firmware détecte et s’adapte automatiquement à un écran OLED SH1106 (128x64, monochrome) ou un LCD TFT ST7789 (240x240 ou 240x320, couleur) au démarrage. Aucun changement de code n’est nécessaire.

## Quelles sont les principales différences entre les environnements OLED et LCD ?
- **OLED (SH1106)** : 2 boutons + encodeur rotatif, monochrome, 128x64, navigation = 2 crans par page.
- **LCD (ST7789)** : 3 boutons + encodeur rotatif, couleur, 240x240/320, navigation = 1 cran par page.
L’interface et la disposition des pages s’adaptent automatiquement à l’afficheur détecté.

## Comment la navigation diffère-t-elle entre OLED et LCD ?
Sur OLED, il faut tourner l’encodeur de deux crans pour changer de page ; sur LCD, un seul cran suffit. Le mapping des boutons diffère aussi (voir guide utilisateur et câblage matériel).

## Puis-je passer d’un OLED à un LCD sans changer le code ?
Oui. Le firmware auto-détecte l’afficheur connecté à chaque démarrage.

## Puis-je changer le contraste de l'OLED ?
Oui, modifiez `OLED_CONTRAST` dans `include/config.h`.

## Où puis-je changer le mappage des broches ?
Dans `include/board_config.h`.

## Que faire si le téléversement échoue à plusieurs reprises ?
Essayez un autre câble USB, vérifiez le port COM, fermez le moniteur série, puis réessayez de téléverser.