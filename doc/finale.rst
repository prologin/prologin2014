============================
Finale Prologin 2014 − Sujet
============================

------------
Introduction
------------

Le sujet de la finale de Prologin 2014 est un jeu de stratégie divisé en
plusieurs phases. Le but du jeu est de vaincre les joueurs ennemis et de
prendre possession de l'artefact central.


Nombre de joueurs
=================

Une partie voit s'affronter 4 joueurs à la fois (1-4).
Chaque joueur possède une base placée dans un coin d'une carte carré de
dimension 81x81.
Au centre de la carte se trouve l'artefact magique (A) et des fontaines
magiques (F) sont disposées entre chaque joueur.


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

*(La carte n'est pas à l'échelle.)*

La carte a une taille constante (longueur et largeur).

Légende :

* ``1..4`` : Joueurs 1 à 4
* ``.`` : Case vide
* ``A`` : Artefact
* ``F`` : Fontaine


But du jeu
==========

Le but du jeu est de totaliser un maximum de points à la fin d’une partie.
Les différents moyens de gagner des points sont :

* Survivre à la fin de la partie (1 point) ;
* Conquérir le camp d’un joueur ennemi (1 point) ;
* Contrôler une fontaine de magie à la fin de la partie (1 point) ;
* Contrôler l’artefact à la fin de la partie (4 points).


Magie
=====

La magie est l'unité de base du jeu.
Vous gagnez de la magie chaque tour, en tuant les unités adverses et en
contrôlant les fontaines et des artefacts. Vous pouvez aussi récupérer de la
magie en supprimant vos propres tourelles, et en dépenser en construisant
des tourelles et en produisant des unités.


Déroulement du jeu
==================

Chaque phase se déroule de manière simultanée pour tous les joueurs : tout le
monde exécute ses actions en même temps. À la phase suivante, vous voyez le
résultat des actions des autres joueurs.  Il n'existe aucun conflit d'action
possible (par exemple, deux joueurs qui poseraient deux tours au même endroit).


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
et des sorciers.

Les sorciers sont toujours construits dans la base du joueur.


Les tours peuvent être construites sur toutes les cases (non spéciales) de la
carte, aux conditions suivantes :

* Elles doivent être proches d'une autre tour du même joueur (être dans la
  "portée de construction" d'une tour)
* Elles doivent être (strictement) plus proches d'une tour du même joueur que
  d'une tour d'un joueur ennemi.

Les tours ont une portée d'attaque minimale de base, mais cette portée peut
être augmentée lors de la construction, selon une formule quadratique.

Il est également possible de détruire vos propres tours pendant cette phase, si
elles vous bloquent le passage. Vous récupérez de la magie à chaque tour
détruite.

Déplacement
===========

Lors de la phase de déplacement, on peut choisir de bouger un certain nombre de
sorciers d'une case à une autre.
Vous avez juste à renseigner le départ et l'arrivée. Si aucun chemin n'existe
entre ces deux points, la fonction de déplacement renverra une erreur.


Attaque des sorciers
--------------------

À la fin de chaque phase de déplacement, lorsque plusieurs sorciers ennemis se
retrouvent sur la même case, ils s'attaquent automatiquement. Le nombre de
sorciers du second groupe en plus grand nombre est retiré à l'ensemble des
groupes, ce qui fait qu'il ne reste plus qu'un joueur sur la case. En cas
d'égalité entre les deux premiers, tout les sorciers s'entre-tuent donc.

Exemple :

* A: 9 - 7 = 2 unités
* B: 7 - 7 = 0 unités
* C: 3 - 7 = 0 unités

Le joueur restant, s'il existe, gagne un certain nombre de points de magie pour
chaque sorcier retiré aux autres joueurs (ici, 7 * REVENU_SORCIER)


Tir des tourelles
=================

Lors de la phase de tir, les tourelles peuvent attaquer des cases avec un
certain nombre de points d'attaque, jusqu'à ce qu'elles n'aient plus de points
d'attaque. Chaque point d'attaque utilisé correspond à un sorcier en moins sur
la case choisie. Il n'est pas possible d'attaquer des tourelles avec cette
technique.

Tuer des sorciers à distance ne rapporte aucun point de magie.


Siège
=====

Lors de la phase de siège, les sorciers peuvent attaquer les tours qui se
trouvent sur une case adjacente. Chaque tour a un nombre de points de vie à sa
création, et en perd 1 par nombre de sorciers qui l'attaquent à chaque tour.
Elle ne peut en regagner.

Lorsqu'elle n'a plus aucun point de vie, la tour est détruite et laisse la voie
libre aux sorciers.


Capture
-------

À la fin de chaque tour :

* Si un sorcier est sur la base d'un ennemi, il est vaincu, et toutes ses
  unités (tours et sorciers) sont supprimées de la map.
* Si un sorcier est sur une fontaine ou un artefact, il gagne un certain nombre
  de points de magie.


----------------
Fin de la partie
----------------

La partie s'arrête au bout d'un certain nombre de tours fixe.

Score
=====

Le score de chaque joueur est déterminé à la fin de la partie :

- 1 points gagnés pour avoir survécu à la fin de la partie
- 1 points gagnés pour avoir vaincu un adversaire
- 1 points gagnés pour contrôler une fontaine à la fin de la partie
- 4 points gagnés pour contrôler un artefact à la fin de la partie
