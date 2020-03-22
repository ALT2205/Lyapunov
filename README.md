# Lyapunov

## Impératifs:
- Écrire du code propre et indenté
- Commenter le code lorsque c'est nécessaire
- Autodocumentation (noms des variables et fonctions explicites)

## Étapes du projet
- Afficher des fractales entre 0 et 4
- Zoom et déplacement
- Images animées (zoom), représentation 3D, parallélisation

## To Do
- ~~Trouver les bornes supérieures/inférieures autres que 0/4~~
-  <span style="color: #0000FF"> Choisir l'orientation de l'image : Baptiste</span>
- ~~Multi threading : voir l'implementation avec <thread> depuis C++11~~
-  <span style="color: #0000FF">Régler les couleurs pour le degradé  : Baptiste</span>
- <span style="color: #00FF00">Essayer de trouver une méthode de calcul plus rapide -> Faire le produit puis calculer son log2 avant que le nombre ne soit trop grand: Adrien </span>
- ~~Redimensionner en fonction du coté de la fenêtre ( taille de la zone de rendu = côté le + petit)~~
- ~~Dézoom~~
- <span style="color: #0000FF">Déplacement : Baptiste</span>
- <span style="color: #0000FF"> Faire en sorte qu'il n'y ait pas besoin de recalculer le contenu s'il a déjà été calculé lors d'un autre déplacement ?: Baptiste </span>
- <span style="color: #FFFF00">Menu pour décider des paramètres techniques (précision de l'image, taille de la texture...): Antoine/Adrien </span>
- <span style="color: #00FF00">Libérer un thread pour gérer les évènements (ex: carré autour de la souris qui ne bouge pas lors de la génération):Adrien </span>
- ~~Commenter le code~~
- <span style="color: #0000FF">Régler la taille du zoom: Baptiste </span>
- Exporter une image et une vidéo - A définir
- <span style="color: #0000FF">Aliasing: Baptiste </span>

## Bibliothèques graphiques:
- SDL2
- GTK + ??

## Date de rendu

15 Mai 2020