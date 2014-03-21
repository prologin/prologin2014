/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2014 Prologin
*/

#ifndef CONSTANT_HH_
# define CONSTANT_HH_

///
// Taille du terrain (longueur et largeur)
//
# define TAILLE_TERRAIN            81

///
// Nombre de joueurs dans la partie
//
# define NB_JOUEURS                4

///
// Magie gagnée à chaque tour
//
# define MAGIE_TOUR                20

///
// Magie gagnée à chaque tour pour chaque fontaine possédée
//
# define MAGIE_FONTAINES           10

///
// Magie gagnée à chaque sorcier tué
//
# define MAGIE_COMBAT              1

///
// Magie gagnée à chaque tourelle détruite
//
# define MAGIE_TOURELLE            3

///
// Magie récupérée à chaque tourelle supprimée
//
# define MAGIE_SUPPRESSION         3

///
// Nombre de points de magie par sorcier
//
# define COUT_SORCIER              2

///
// Nombre de points de magie par sorcier
//
# define COUT_TOURELLE             6

///
// Coût pour chaque case de portée supplémentaire
//
# define COUT_PORTEE               2

///
// Portée de base d'une tourelle
//
# define PORTEE_TOURELLE           4

///
// Portée de construction des tourelles
//
# define CONSTRUCTION_TOURELLE     5

///
// Points de vie d'une tourelle à sa création
//
# define VIE_TOURELLE              10

///
// Points d'attaque d'une tourelle au début d'un tour
//
# define ATTAQUE_TOURELLE          10

///
// Points gagnés pour avoir survécu à la fin de la partie
//
# define POINTS_SURVIVRE           1

///
// Points gagnés pour avoir vaincu un adversaire
//
# define POINTS_VAINQUEUR          1

///
// Points gagnés pour contrôler une fontaine à la fin de la partie
//
# define POINTS_CONTROLE_FONTAINE  1

///
// Points gagnés pour contrôler un artefact à la fin de la partie
//
# define POINTS_CONTROLE_ARTEFACT  4

///
// Information sur les cases
//
typedef enum case_info {
  CASE_SIMPLE, /* <- Case simple */
  CASE_TOURELLE, /* <- Tourelle */
  CASE_CASE, /* <- Base du joueur */
  CASE_FONTAINE, /* <- Fontaine magique */
  CASE_ARTEFACT, /* <- Artefact magique */
  CASE_ERREUR, /* <- Erreur */
} case_info;


///
// Erreurs possibles
//
typedef enum erreur {
  OK, /* <- L'action s'est effectuée avec succès */
  ANNULER_IMPOSSIBLE, /* <- Aucune action à annuler */
  CASE_IMPOSSIBLE, /* <- Cette case n'existe pas */
  CASE_ADVERSE, /* <- Vous ne contrôlez pas cette case */
  CASE_UTILISEE, /* <- Cette case n'est pas libre */
  CASE_VIDE, /* <- Cette case est vide */
  VALEUR_INVALIDE, /* <- Cette valeur est invalide */
  MAGIE_INSUFFISANTE, /* <- Vous n'avez pas assez de magie */
  SORCIERS_INSUFFISANTS, /* <- Vous n'avez pas assez de sorciers */
  ATTAQUE_INSUFFISANTE, /* <- Vous n'avez pas assez de points d'attaque */
} erreur;


///
// Représente la position sur la carte
//
typedef struct position {
  int x;  /* <- Coordonnée en X */
  int y;  /* <- Coordonnée en Y */
} position;


///
// Représente une tourelle
//
typedef struct tourelle {
  position pos;  /* <- Position de la tourelle */
  int portee;  /* <- Portée de la tourelle */
  int joueur;  /* <- Joueur qui possède la tourelle */
  int vie;  /* <- Nombre de points de vie de la tourelle */
  int magie;  /* <- Nombre de points de magie de la tourelle */
} tourelle;



#endif // !CONSTANT_HH_
