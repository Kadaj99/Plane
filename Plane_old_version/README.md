# Projet de LIFAPCD

Ce projet est un jeu de tir 2D simple dans lequel le joueur contrôle un avion pour éviter et détruire les ennemis tout en collectant des power-ups pour améliorer ses armes.

## Installation

1. Installez les dépendances nécessaires :
   - SDL2
   - SDL2_image
   - SDL2_ttf
   - SDL2_mixer

2. Compilez le projet en utilisant `make` ou en exécutant les commandes de compilation appropriées.

3. Exécutez le fichier binaire généré pour lancer le jeu.

## Contrôles

- Flèches directionnelles : Déplacez l'avion du joueur
- Barre d'espace : Tirer des balles
- Touche P : Mettre le jeu en pause
- Touche Q : Quitter le jeu
- Touche Esc : Retour au menu principal
- Touche G : Tir automatique de balles


## Fonctionnalités

- Différents types d'ennemis avec des comportements et des vitesses variables
- Collectez des power-ups pour améliorer les armes du joueur
- Différentes trajectoires de balles pour les améliorations d'armes
- Système de score basé sur le nombre d'ennemis détruits
- Pause et reprise du jeu

## À propos du code

Le projet est principalement écrit en C++ et utilise la bibliothèque SDL2 pour la gestion des graphismes, des événements et du son. Le code est organisé en plusieurs classes pour gérer différentes parties du jeu, telles que le joueur, les ennemis, les balles et les power-ups.

## Problèmes connus et améliorations possibles

- Des objets de ravitaillement pourraient être lâchés de manière aléatoire pour améliorer l'expérience de jeu du joueur.
- Achats en boutique pour améliorer la vitesse de lancement des balles et les dégâts d'attaque en dépensant des points de joueur (une fois que la fonction de sang est ouverte).
- Générer du sang ennemi et du sang de joueur pour augmenter la difficulté du jeu.
- Générer des ennemis avancés pour enrichir le jeu
- Affichage du nombre de dégâts lors de l'attaque des ennemis
- La génération des ennemis et des power-ups pourrait être améliorée pour offrir une expérience de jeu plus variée.
- La gestion des collisions pourrait être améliorée pour une détection plus précise entre les objets du jeu.
- Les performances pourraient être optimisées pour fonctionner plus efficacement sur différents types de matériel.

## Licence

Ce projet est sous licence MIT.