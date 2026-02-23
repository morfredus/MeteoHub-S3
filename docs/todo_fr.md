# Feuille de route TODO OLED et UX

Version minimale valide : 1.0.159

## TODO code
- [x] Renommer `Sh1106Display` vers un nom neutre (`OledDisplay`) pour refléter correctement le backend U8g2 actuel qui prend en charge SH1106 et SSD1306.
- [x] Centraliser les constantes de géométrie OLED (`largeur`, `hauteur`, marges) pour supprimer les valeurs magiques dans `pages_oled.cpp`.
- [ ] Ajouter une petite API de diagnostic d’affichage (FPS courant, durée du dernier rendu, compteur d’erreurs I2C) exposée dans `/api/system`.
- [ ] Ajouter un flag de build optionnel pour compiler uniquement un backend contrôleur OLED (`SH1106` ou `SSD1306`) afin de réduire la taille binaire.
- [ ] Ajouter des tests unitaires côté hôte pour les fonctions utilitaires pures de `pages_oled.cpp` (formatage, génération des titres, traduction des alertes).
- [ ] Remplacer les attentes UI bloquantes (`delay(1000/2000)`) dans les parcours OLED par une machine d’état non bloquante basée sur `millis()`.
- [ ] Introduire un helper/macro de yield coopératif partagé (`delay(0)` toutes N itérations) et l’utiliser de manière homogène dans les longues boucles historique/fichiers.
- [ ] Réduire les allocations fréquentes de `std::string` dans les chemins de rendu OLED en utilisant des buffers de formatage réutilisables sur les pages critiques.
- [ ] Ajouter des politiques de rafraîchissement intelligentes selon le type de page (rafraîchir seulement si changement sur pages statiques, plus rapide sur pages live).

## TODO expérience utilisateur
- [ ] Ajouter une page de réglages OLED rapide (contraste + aperçu adresse I2C) directement sur l’appareil.
- [ ] Ajouter un assistant de premier démarrage OLED qui valide le câblage (SDA/SCL/adresse) et affiche des étapes claires si aucun écran n’est détecté.
- [ ] Ajouter un toast/ligne de statut non bloquant lors des tentatives de reconnexion SD pour expliciter les échecs temporaires de sauvegarde.
- [ ] Ajouter un réglage de vitesse de transition des pages et de densité du graphe pour améliorer la lisibilité selon les usages.
- [ ] Ajouter une aide contextuelle sur appui long (légende/icônes/indices selon l’écran).
- [ ] Ajouter des repères de navigation persistants à l’écran (`OK/Menu`, `Retour`, `Rotation`) sur les pages OLED compactes.
- [ ] Mettre en pause l’auto-rotation des vues de prévision pendant quelques secondes après une interaction utilisateur pour améliorer la lecture.
- [ ] Améliorer le rendu `Ciel :` avec un dictionnaire d’abréviations plus riche et une troncature sur mots entiers.
- [ ] Ajouter une page de statut rapide OLED dédiée (`Wi-Fi`, `NTP`, `SD`, état API) pour le diagnostic terrain sans accès web.
- [ ] Définir et documenter un profil produit « OLED minimal + web avancé » pour garder l’OLED synthétique et déplacer les parcours détaillés vers la web UI.

## Rendu UTF-8 OLED

Depuis la version 1.0.155, le backend OLED (`OledDisplay`) prend en charge le rendu UTF-8 pour tous les textes, y compris les caractères accentués et symboles spéciaux. Le module d'affichage porte désormais le nom neutre `OledDisplay` pour refléter sa compatibilité avec SH1106 et SSD1306.
