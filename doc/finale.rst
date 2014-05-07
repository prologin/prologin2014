============================
Finale Prologin 2014 − Sujet
============================

------------
Introduction
------------

Le sujet de la finale de Prologin 2014 est un jeu de stratégie divisé en
plusieurs phases. Le but du jeu est de vaincre les joueurs ennemis et de
prendre possession de l'artefact central et des fontaines magiques.


Nombre de joueurs
=================

Une partie voit s'affronter 4 joueurs à la fois (1-4).
Chaque joueur possède une base placée dans un coin d'une carte carrée de
dimensions 31 x 31.
Au centre de la carte se trouve l'artefact magique (A) et des fontaines
magiques (F) sont disposées au nord, sud, est et ouest de la salle.
Les cases sur lesquelles sont les bases, les fontaines et l'artefact sont des
cases spéciales.


Carte
=====

Schéma::

    1 . . . . F . . . . 2
    . . . . . . . . . . .
    . . . . . . . . . . .
    . . . . . . . . . . .
    . . . . . . . . . . .
    F . . . . A . . . . F
    . . . . . . . . . . .
    . . . . . . . . . . .
    . . . . . . . . . . .
    . . . . . . . . . . .
    3 . . . . F . . . . 4

*(Le schéma n'est pas à l'échelle.)*

La carte a une taille constante (longueur et largeur) : TAILLE_TERRAIN par TAILLE_TERRAIN.

Légende :

* ``1..4`` : Joueurs 1 à 4
* ``.`` : Case vide
* ``A`` : Artefact
* ``F`` : Fontaine


But du jeu
==========

Le but du jeu est de totaliser un maximum de points à la fin d'une partie.
Les différents moyens de gagner des points sont :

* survivre à la fin de la partie (1 point) ;
* conquérir le camp d'un joueur ennemi (1 point) ;
* contrôler une fontaine de magie à la fin de la partie (1 point) ;
* contrôler l'artefact à la fin de la partie (4 points).


Magie
=====

La magie est l'unité de base du jeu.
À chaque tour, vous gagnez MAGIE_TOUR points de magie de base,
et MAGIE_FONTAINES par fontaine contrôlée. On gagne aussi MAGIE_COMBAT point de magie
par adversaire tué.
Vous pouvez aussi récupérer MAGIE_SUPPRESSION par tourelle en votre possession que vous supprimez, et en dépenser en construisant
des tourelles (COUT_TOURELLE par tourelle) et en produisant des sorciers (COUT_SORCIER par sorcier).


Déroulement du jeu
==================

Chaque phase se déroule de manière simultanée pour tous les joueurs : tout le
monde exécute ses actions en même temps. À la phase suivante, vous voyez le
résultat des actions des autres joueurs.  Il n'existe aucun conflit d'action
possible.


------
Phases
------

Chaque tour se déroule en 4 phases :

* 1) construction
* 2) déplacement
* 3) tir des tourelles
* 4) siège


Construction
============

Lors de la phase de construction, il est possible de construire des tourelles
et des sorciers pour un coût COUT_TOURELLE et COUT_SORCIER.

Les sorciers sont toujours construits dans la base du joueur.


Une tourelle peut être construite sur une case vérifiant les conditions suivantes :
* ce n'est pas une case spéciale
* cette case doit être proche d'une autre tour du même joueur (être dans la « portée de construction » d'une tour) ;
* le joueur doit avoir une tour qui est strictement plus proche de cette case que n'importe
quelle tour d'un joueur ennemi (c'est pourquoi deux joueurs ne risquent pas de construire
une tour au même endroit).


Les tours ont une portée d'attaque minimale de base PORTEE_TOURELLE, mais cette portée peut
être augmentée lors de la construction : pour augmenter la portée de base de X
cases, on paie un surcoût de COUT_PORTEE + X*X. On paie donc au total
COUT_TOURELLE + COUT_PORTEE + X*X.

Il est également possible de détruire vos propres tourelles pendant cette
phase, si elles vous bloquent le passage. Vous récupérez MAGIE_SUPPRESSION points de magie pour chaque tourelle
vous appartenant que vous détruisez.

Déplacement
===========

Lors de la phase de déplacement, on peut choisir de bouger un certain nombre
de sorciers d'une distance PORTEE_SORCIER. Cependant, un sorcier
ne peut pas se rendre sur une case où il y a une tourelle.


Attaque des sorciers
--------------------

À la fin de chaque phase de déplacement, lorsque plusieurs sorciers ennemis se
retrouvent sur la même case, ils s'attaquent automatiquement. Le nombre de
sorciers du deuxième groupe en plus grand nombre est retiré à l'ensemble des
groupes, ce qui fait qu'il ne reste plus que les soldats d'un unique joueur
sur la case, ou bien aucun soldat (en cas d'égalité des deux groupes les plus
nombreux).

Exemple : si A, B, C, D ont respectivement 1, 3, 3 et 7 soldats, à l'issue du
combat :

* A n'a plus que 7 - 3 = 4 unités ;
* B, C et D : 0 unité.

Le joueur restant, s'il existe, gagne REVENU_SORCIER points de magie
pour chaque sorcier retiré aux autres joueurs (ici, (1 + 3 + 3) *
REVENU_SORCIER)


Tir des tourelles
=================

Lors de la phase de tir, les tourelles peuvent répartir leurs ATTAQUE_TOURELLE points d'attaque
sur un ensemble de cases. Chaque point d'attaque utilisé correspond à un sorcier en moins sur
la case choisie. Il n'est pas possible d'attaquer des tourelles avec cette
technique.

Tuer des sorciers à distance ne rapporte aucun point de magie.


Siège
=====

Lors de la phase de siège, les sorciers peuvent attaquer les tourelles qui se
trouvent sur une case adjacente (haut, bas, gauche, droite). Chaque tourelle a
VIE_TOURELLE de points de vie à sa création, et en perd 1 par nombre de sorciers
qui l'attaquent à chaque tour. Elle ne peut pas en regagner.

Lorsqu'elle n'a plus aucun point de vie, la tourelle est détruite et laisse
la voie libre aux sorciers.


Capture
-------

À la fin de chaque tour\footnote{Et non tourelle, vous suivez ?} :

* Si un sorcier est sur la base d'un ennemi, ce dernier est vaincu, et toutes
  ses unités (tourelles et sorciers) sont supprimées de la carte.
* Si un sorcier est sur une fontaine, il fait gagner MAGIE_FONTAINES points de magie au joueur qui le contrôle.


----------------
Fin de la partie
----------------

La partie s'arrête au bout de MAX_TOUR tours.

Score
=====

Le score de chaque joueur est déterminé à la fin de la partie comme expliqué dans la partie « But du jeu » ci-dessus.