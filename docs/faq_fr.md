# FAQ

Version minimale valide : 1.0.20

## Internet est-il obligatoire ?
Oui, pour les mises à jour de prévisions et d’alertes. Les pages capteurs locales restent utilisables sans internet.

## Puis-je utiliser un seul capteur ?
Oui. Le projet supporte la disponibilité partielle des capteurs et des comportements de secours.

## Pourquoi les graphes sont vides après redémarrage ?
L’historique a besoin d’échantillons temporels et d’une heure valide (NTP) pour enregistrer des entrées.

## Quelle est la fréquence de mise à jour des prévisions ?
Environ toutes les 30 minutes.

## Combien de points d’historique sont stockés ?
128 points sont conservés en mémoire et persistés dans LittleFS.

## Puis-je changer la vitesse de rafraîchissement des pages ?
Oui, modifier `DASHBOARD_REFRESH_MS` dans `include/config.h`.

## Puis-je changer le contraste OLED ?
Oui, modifier `OLED_CONTRAST` dans `include/config.h`.

## Où modifier le mapping des pins de la carte ?
Dans `include/board_config.h`.

## Que faire si l’upload échoue de manière répétée ?
Essayer un autre câble USB, vérifier le port COM, fermer le moniteur série, puis relancer l’upload.
