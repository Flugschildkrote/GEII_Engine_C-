/////////////////////////////////////////////////////////////////////////  
///////////////////////////[INFO VERSION]////////////////////////////////  
/////////////////////////////////////////////////////////////////////////  

Version C++ : C++14  
Compilateur: mingw32 inclus dans GCC 7.3.0  
Version OpenGL : 3.3  
Version GLM  : 0.9.8.5 	(gestion des outils mathématiques)  
  
Version GLFW : 3.2.1   	(gestion des fenêtres / contextes OpenGL)  
Version GLEW : 2.1.0	(gestion des extensions OpenGL)  

/////////////////////////////////////////////////////////////////////////  
/////////////////////////////////////////////////////////////////////////  
/////////////////////////////////////////////////////////////////////////  
   
Commandes :  
  
[CAMERA]  
Déplacer la caméra : Flèches directionnelles  
Tourner la caméra : Déplacer la souris  
  
[LUMIERE]  
Faire tourner la lumière : CTRL+Flèches directionnelles  
  
[OBJETS]  
Cacher un objet : Clic sur l'objet  
Déplacement du chat (axes locaux) : Numpad(4, 5, 6, 8, +, -)  
Déplacement du chat (axes globaux) : CRTL+Numpad(4, 5, 6, 8, +, -)  
Rotation du chat (axes locaux) : ALT+Numpad(4, 5, 6, 8, +, -)  
Rotation du chat (axes globaux) : ALT+CRTL+Numpad(4, 5, 6, 8, +, -)  
  
[CURSEUR]  
Vérouiller le curseur : 1  
Dévérouiller le curseur : 2  
/////////////////////////////////////////////////////////////////////////  
/////////////////////////////////////////////////////////////////////////  
/////////////////////////////////////////////////////////////////////////  

[09/11/2018]  
Création des éléments princpaux du moteur 3D.  
Ajout de la gestion des skybox.  
  
[10/11/2018]  
Suppression de la classe ImageLoader.  
Intégration de stb_image pour charger les images.  
  
[11/11/2018]  
Ajout de la gestion des évenements (souris, clavier, redimmensionnement de la fenêtre)  
  
[12/11/2018]  
Ajout de fonctionnalités à InputManager  
Ajout du Picking  
  
[14/11/2018]  
Ajout de la gestion des textures vides  
Implémentation des Frame Buffer Object  
  
[15/11/2018]  
Implémentation d'un système de Shadow Mapping  
  
[18/11/2018]  
Ajout de la classe Transform pour gérer tous les déplacements  