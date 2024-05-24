# ProjetInfo-CY-P1MI3A

Projet d'informatique pour la fin de notre année en Pré ING 1  
Fait par le trinôme A du groupe MI3  
Sujet : Cosmic Yonder

## Contributeurs

- Romain MICHAUT--JOYEUX
- Kevin NGUYEN OANH
- Mathis SINET

## Utilisation

Pour compiler la solution, utiliser `make`.

Pour exécuter la solution, utiliser `make run`.

Pour nettoyer les fichiers de construction, utiliser `make clean`.

## Description du programme

Lors du lancement du programme, vous vous retrouverez dans un menu principal proposant 3 options :

- Lancer une nouvelle partie
- Charger une partie
- Quitter la partie

Lorsque vous choisissez l'option `Lancer la partie`, le programme vous demandera le nom que vous voulez donner au personnage ainsi que la graine. Une histoire se lancera par la suite. Puis, vous allez incarner un personnage qui devra défendre sa station spatiale.

- Pour cela, vous devez accomplir 3 quêtes:

  - Ramasser une peluche et un ballon de foot (ces objets ont un rapport avec la vie de votre personnage)
  - Tuer un certain nombre de monstres
  - Répondre à 3 questions de type quizz en rapport avec le programme d'informatique du 2nd semestre (chaque question aura un niveau de difficulté différent). Ces questions ont le même style que les quizz sur le Moodle de CY Tech.

- Durant la partie, vous pouvez la sauvegarder avec la touche `W` ou la quitter avec `X`. Vous pouvez également avoir la liste des commandes avec la touche `C`.

- Vous avez 10 minutes pour finir la partie. Si vous mourez avant le temps imparti, un message apparaîtra sur l'écran disant que vous vous êtes fait tuer. Dans le cas contraire :

  - Si vous arrivez à accomplir les quêtes avant le temps imparti, un message apparaîtra indiquant que vous aurez gagné.
  - Sinon, un message apparaîtra indiquant que vous aurez échoué.

Lorsque vous choisissez l'option `Charger une partie`, le programme vous demandera de saisir le nom du joueur.

- Si le programme trouve une sauvegarde au nom que vous avez saisi, la partie se lancera avec tout ce que vous avez au moment de la sauvegarde (inventaire, points de vie, défense, avancement des quêtes).
- Dans le cas contraire, un message s'affichera indiquant qu'il y a aucune sauvegarde au nom que vous avez saisi. Le porgramme vous renverra donc au menu principal.

Lorsque vous choisissez l'option `Quitter la partie`, le programme s'arrêtera.

## Contenu

La partie `display.c` gère l'aspect graphique du jeu (fenêtres, affichage de la barre de vie et de défense, inventaire, affichage de l'histoire) mais aussi à l'écran de fin de jeu (victoire, défaite). Elle gère également la gestion de l'inventaire.

La partie `game.c` gère les déplacements du personnage que l'on incarne ainsi que leur système d'attaque (vitesse d'attaque, dégâts). Elle gère également les déplacements des monstres (déplacement en fonction de la position du personnage, vitesse de déplacement) ainsi que leur système de combat (vitesse d'attaque, dégâts, animation de mort).

La partie `gamebasic.c` possède la définition des structures de base du jeu ainsi que des fonctions de base permettant de les manipuler.

La partie `item.c` correspond aux informations de tous les éléments avec lesquels on peut intéragir. Elle contient les statistiques des différents objets comme les objets de soin ou encore les armes.

La partie `main.c` exécute toutes les fonctions de tous les fichiers. Elle possède des fonctions vérifiant la bonne éxécution du programme.

La partie `mapgen.c` gère l'emplacement des murs et des portes en fonction de la seed. Elle gère également la mise en place des objets et des monstres dans différentes salles et l'allocation des salles. Elle contient la fonction d'initialisation d'une partie.

La partie `monster.c` correspond aux informations les monstres. Elle possède les statistiques des monstres ainsi que leur aptitudes de combat.

La partie `quest.c` possède la génération des quizz et la fonction vérifiant que les quêtes sont terminées.

La partie `save.c` correspond à la gestion des sauvegardes des parties.

La partie `settings.c` contient plusieurs constantes importantes utilisées dans le programme.

## Bibliothèques utilisées

`ncursesw`
