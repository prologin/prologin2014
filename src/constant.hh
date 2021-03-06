/*
** Copyright (c) 2014 Mélanie Godard <melanie.godard@prologin.org>
** Copyright (c) 2014 Antoine Pietri <antoine.pietri@prologin.org>
** Copyright (c) 2014 Association Prologin <info@prologin.org>
**
** prologin2014 is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** prologin2014 is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with prologin2014.  If not, see <http://www.gnu.org/licenses/>.
*/

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
#define CONSTANT_HH_

///
// Taille du terrain (longueur et largeur)
//
#define TAILLE_TERRAIN 31

///
// Nombre de joueurs dans la partie
//
#define NB_JOUEURS 4

///
// Nombre maximum de tours à jouer avant la fin de la partie
//
#define MAX_TOUR 100

///
// Magie gagnée à chaque tour
//
#define MAGIE_TOUR 20

///
// Magie gagnée à chaque tour pour chaque fontaine possédée
//
#define MAGIE_FONTAINES 15

///
// Magie gagnée à chaque sorcier tué
//
#define MAGIE_COMBAT 1

///
// Magie récupérée à chaque tourelle supprimée
//
#define MAGIE_SUPPRESSION 10

///
// Nombre de points de magie par sorcier
//
#define COUT_SORCIER 2

///
// Nombre de points de magie par tourelle
//
#define COUT_TOURELLE 20

///
// Coût exponentiel pour chaque case de portée supplémentaire
//
#define COUT_PORTEE 4

///
// Nombre maximum de cases qu'un sorcier peut franchir à chaque tour.
//
#define PORTEE_SORCIER 4

///
// Portée de base d'une tourelle
//
#define PORTEE_TOURELLE 3

///
// Portée de construction des tourelles
//
#define CONSTRUCTION_TOURELLE 3

///
// Points de vie d'une tourelle à sa création
//
#define VIE_TOURELLE 10

///
// Points d'attaque d'une tourelle au début d'un tour
//
#define ATTAQUE_TOURELLE 10

///
// Points gagnés pour avoir survécu à la fin de la partie
//
#define POINTS_SURVIVRE 1

///
// Points gagnés pour avoir vaincu un adversaire
//
#define POINTS_VAINQUEUR 1

///
// Points gagnés pour contrôler une fontaine à la fin de la partie
//
#define POINTS_CONTROLE_FONTAINE 1

///
// Points gagnés pour contrôler un artefact à la fin de la partie
//
#define POINTS_CONTROLE_ARTEFACT 4

///
// Information sur les cases
//
typedef enum case_info
{
    CASE_SIMPLE,   /* <- Case simple */
    CASE_TOURELLE, /* <- Tourelle */
    CASE_BASE,     /* <- Base du joueur */
    CASE_FONTAINE, /* <- Fontaine magique */
    CASE_ARTEFACT, /* <- Artefact magique */
    CASE_ERREUR,   /* <- Erreur */
} case_info;

///
// Erreurs possibles
//
typedef enum erreur
{
    OK,                    /* <- L'action s'est effectuée avec succès */
    ANNULER_IMPOSSIBLE,    /* <- Aucune action à annuler */
    CASE_IMPOSSIBLE,       /* <- Cette case n'existe pas */
    CASE_ADVERSE,          /* <- Vous ne contrôlez pas cette case */
    CASE_UTILISEE,         /* <- Cette case n'est pas libre */
    CASE_VIDE,             /* <- Cette case est vide */
    VALEUR_INVALIDE,       /* <- Cette valeur est invalide */
    MAGIE_INSUFFISANTE,    /* <- Vous n'avez pas assez de magie */
    SORCIERS_INSUFFISANTS, /* <- Vous n'avez pas assez de sorciers */
    ATTAQUE_INSUFFISANTE,  /* <- Vous n'avez pas assez de points d'attaque */
    PHASE_INCORRECTE, /* <- Cette action ne peut pas être utilisée lors de cette
                         phase du jeu. */
    PORTEE_INSUFFISANTE, /* <- Vous n'avez pas assez de portée pour effectuer
                            cette action */
    PERDANT, /* <- Vous avez perdu et ne pouvez pas effectuer d'actions */
} erreur;

///
// Représente la position sur la carte
//
typedef struct position
{
    int x; /* <- Coordonnée en X */
    int y; /* <- Coordonnée en Y */
} position;

///
// Représente une tourelle
//
typedef struct tourelle
{
    position pos; /* <- Position de la tourelle */
    int portee;   /* <- Portée de la tourelle */
    int joueur;   /* <- Joueur qui possède la tourelle */
    int vie;      /* <- Nombre de points de vie de la tourelle */
    int attaque;  /* <- Nombre de points d'attaque de la tourelle */
} tourelle;

#endif // !CONSTANT_HH_
