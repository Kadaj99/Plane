# Projet de LIFAPCD
DogFighter est un jeu-vidéo developpé en c++ en utilisant la SDL/SDL2. Ce projet de developpement à été réalisé dans le cadre de l'UE LIFAP4
Voir la page du cours :  https://perso.liris.cnrs.fr/alexandre.meyer/public_html/www/doku.php?id=lifap4

Ce dossier contient un makefile permettant d'executer le code du jeu-vidéo.
le Dossier src contient les sous-dossiers /core /sdl2 et /txt qui contiennent les modules spécifiques du jeu. (fichiers .h .cpp .txt) 

Il peut se compiler sous Linux/ MacOS et Windows (avec WSL notamment) , avec la commande make depuis le dossier source(F9 sur Code::blocks).

## Installation et Compiler

 ### Sous Linux

 Si vous êtes sous Linux, assurez-vous d'avoir SDL/SDL2 installé sur votre machine.
 - Ouvrez un terminal et exécutez simplement la commande : `sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev imagemagick`
 - Puis, depuis le dossier racine du jeu, exécutez, depuis votre terminal, la commande : `make`
 - Une fois la compilation terminée, exécutez la commande `./bin/Debug/Plane` pour lancer du jeu

- Remarque : Sous linux, l'instruction `CXX=g++` dans le Makefile
 Si la commande d'installation de la SDL/SDL2 ne fonctionne pas, référez-vous à la commande spécifique pour installer SDL/SDL2 selon votre distribution Linux.

 ### Sous macOS

 Pour compiler sous macOS, assurez-vous d'avoir SDL/SDL2 installé sur votre machine.
 - Installez Homebrew si ce n'est pas déjà fait : `/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"`
 - Installez les bibliothèques SDL2 : `brew install sdl2 sdl2_image sdl2_ttf sdl2_mixer`
 - Puis, depuis le dossier racine du jeu, exécutez la commande : `make`
 - Une fois la compilation terminée, exécutez la commande `./bin/Debug/Plane` pour lancer du jeu

- Remarque : Sous linux, l'instruction `CXX=clang++` dans le Makefile

 ### Sous Windows avec Visual Studio

 Pour compiler sous Windows avec Visual Studio, assurez-vous d'avoir SDL/SDL2 installé sur votre machine.
 - Téléchargez les bibliothèques SDL2 (https://www.libsdl.org/download-2.0.php) et décompressez-les
 - Créez un nouveau projet Visual Studio C++ et ajoutez les fichiers sources du jeu
 - Configurez les propriétés du projet pour inclure les répertoires d'en-tête et les bibliothèques de SDL2
 - Compilez le projet et exécutez le jeu depuis Visual Studio

 ### Sous Windows avec Code::Blocks

 Pour compiler sous Windows avec Code::Blocks, assurez-vous d'avoir SDL/SDL2 installé sur votre machine.
 - Téléchargez les bibliothèques SDL2 (https://www.libsdl.org/download-2.0.php) et décompressez-les
 - Créez un nouveau projet Code::Blocks C++ et ajoutez les fichiers sources du jeu
 - Configurez les propriétés du projet pour inclure les répertoires d'en-tête et les bibliothèques de SDL2
 - Compilez le projet (F9) et exécutez le jeu depuis Code::Blocks

### Préambule : si vous voulez un Linux sous Windows, vous pouvez installer WSL !

Voir la page de MicroSoft : https://learn.microsoft.com/en-us/windows/wsl/install

C'est léger, pratique, rapide et suffisant pour de nombreuses applications.
Windows/WSL est exactement comme une machine Linux avec des drivers minimalistes,
donc la gestion de certains périphériques commes la carte son ou l'accélération 3D peut ne pas marcher mais pour ce cours ca peut être suffisant.
Il faut parcontre que vous ayez installé XMing pour pouvoir ouvrir des fenêtres comme expliqué sur la page du cours.
Valgrind marche aussi !


 ### Pour installer SDL2
 Afin d'installer SDL2 sous Linux ou avec WSL/Windows, veuillez suivre les instructions ci-dessous avec soin :

 ```sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev imagemagick```

 Veuillez noter que cette commande peut varier en fonction de votre distribution Linux. Consultez la documentation de votre distribution pour connaître la commande appropriée.

 ### Compilation en ligne de commande après l'installation de SDL2 et WSL (si nécessaire)
 Une fois SDL2 et WSL (le cas échéant) correctement installés, veillez à effectuer la compilation en ligne de commande avec prudence :

 - Depuis le répertoire racine du module image, exécutez attentivement la commande `make`.
 - Si la compilation ne s'effectue pas comme prévu, vérifiez minutieusement que les chemins d'inclusion spécifiés dans le fichier Makefile à la racine du projet correspondent à votre installation de SDL2 (par défaut `/usr/include`).
 - Adaptez les chemins d'inclusion si nécessaire, en fonction de votre configuration et de l'emplacement de l'installation SDL2.

## Contrôles

- Flèches directionnelles : Déplacez l'avion du joueur
- Touche P : Mettre le jeu en pause
- Touche Q : Quitter le jeu
- Touche Esc : Retour au menu principal
- Touche F : Tir de balles


## Fonctionnalités

- Différents types d'ennemis avec des comportements et des vitesses variables
- Collectez des power-ups pour améliorer les armes du joueur
- Différentes trajectoires de balles pour les améliorations d'armes
- Système de score basé sur le nombre d'ennemis détruits
- Pause et reprise du jeu

## À propos du code

Le projet est principalement écrit en C++ et utilise la bibliothèque SDL2 pour la gestion des graphismes, des événements et du son. Le code est organisé en plusieurs classes pour gérer différentes parties du jeu, telles que le joueur, les ennemis, les balles et les power-ups.

## Problèmes connus et améliorations possibles

- Des objets de ravitaillement pourraient être lâchés de manière aléatoire pour améliorer l'expérience de jeu du joueur.(Succès)
- Achats en boutique pour améliorer la vitesse de lancement des balles et les dégâts d'attaque en dépensant des points de joueur (une fois que la fonction de sang est ouverte).(Succès)
- Générer du sang ennemi et du sang de joueur pour augmenter la difficulté du jeu.(pas encore).
- Générer des ennemis avancés pour enrichir le jeu(pas encore).(pas encore)
- Affichage du nombre de dégâts lors de l'attaque des ennemis。(pas encore)
- La génération des ennemis et des power-ups pourrait être améliorée pour offrir une expérience de jeu plus variée.(Succès)
- La gestion des collisions pourrait être améliorée pour une détection plus précise entre les objets du jeu.(Succès)
- Les performances pourraient être optimisées pour fonctionner plus efficacement sur différents types de matériel.(Succès)

## Licence

Ce projet est sous licence MIT.
