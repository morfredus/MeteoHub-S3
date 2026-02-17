# FAQ

Version minimale valide : 1.0.59

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
- **Long terme** : Jusqu'à 2 ans d'archives stockées dans la mémoire interne (fichiers mensuels, 1 point/15 min).

## Comment récupérer mes données ?
Allez sur l'interface Web, cliquez sur l'icône dossier 📂 en bas de page. Vous pouvez télécharger les fichiers `.dat` (format binaire) ou les logs.

## Puis-je changer la vitesse de rafraîchissement des pages ?
Oui, modifier `DASHBOARD_REFRESH_MS` dans `include/config.h`.


## Le projet gère-t-il à la fois OLED et LCD ?
Oui. Le firmware détecte et s’adapte automatiquement à un écran OLED SH1106 (128x64, monochrome) ou un LCD TFT ST7789 (240x240, couleur) au démarrage. Aucun changement de code n’est nécessaire.

## Quelles sont les principales différences entre les environnements OLED et LCD ?
- **OLED (SH1106)** : 2 boutons + encodeur rotatif, monochrome, 128x64, navigation = 2 crans par page.
- **LCD (ST7789)** : 3 boutons + encodeur rotatif, couleur, 240x240, navigation = 1 cran par page.
L’interface et la disposition des pages s’adaptent automatiquement à l’afficheur détecté.

## Comment la navigation diffère-t-elle entre OLED et LCD ?
Sur OLED, il faut tourner l’encodeur de deux crans pour changer de page ; sur LCD, un seul cran suffit. Le mapping des boutons diffère aussi (voir guide utilisateur et câblage matériel).

## Puis-je passer d’un OLED à un LCD sans changer le code ?
Oui. Le firmware auto-détecte l’afficheur connecté à chaque démarrage.

## Puis-je changer le contraste OLED ?
Oui, modifier `OLED_CONTRAST` dans `include/config.h`.

## Où modifier le mapping des pins de la carte ?
Dans `include/board_config.h`.

## Que faire si l’upload échoue de manière répétée ?
Essayer un autre câble USB, vérifier le port COM, fermer le moniteur série, puis relancer l’upload.
