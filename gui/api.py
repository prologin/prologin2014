# -*- coding: iso-8859-1 -*-

try:
    from _api import *
except ImportError:
    pass

# Taille du terrain (longueur et largeur)
TAILLE_TERRAIN = 31

# Nombre de joueurs dans la partie
NB_JOUEURS = 4

# Nombre maximum de tours à jouer avant la fin de la partie
MAX_TOUR = 100

# Magie gagnée à chaque tour
MAGIE_TOUR = 20

# Magie gagnée à chaque tour pour chaque fontaine possédée
MAGIE_FONTAINES = 15

# Magie gagnée à chaque sorcier tué
MAGIE_COMBAT = 1

# Magie récupérée à chaque tourelle supprimée
MAGIE_SUPPRESSION = 10

# Nombre de points de magie par sorcier
COUT_SORCIER = 2

# Nombre de points de magie par tourelle
COUT_TOURELLE = 20

# Coût exponentiel pour chaque case de portée supplémentaire
COUT_PORTEE = 4

# Nombre maximum de cases qu'un sorcier peut franchir à chaque tour.
PORTEE_SORCIER = 4

# Portée de base d'une tourelle
PORTEE_TOURELLE = 3

# Portée de construction des tourelles
CONSTRUCTION_TOURELLE = 3

# Points de vie d'une tourelle à sa création
VIE_TOURELLE = 10

# Points d'attaque d'une tourelle au début d'un tour
ATTAQUE_TOURELLE = 10

# Points gagnés pour avoir survécu à la fin de la partie
POINTS_SURVIVRE = 1

# Points gagnés pour avoir vaincu un adversaire
POINTS_VAINQUEUR = 1

# Points gagnés pour contrôler une fontaine à la fin de la partie
POINTS_CONTROLE_FONTAINE = 1

# Points gagnés pour contrôler un artefact à la fin de la partie
POINTS_CONTROLE_ARTEFACT = 4


from enum import IntEnum

# Information sur les cases
class case_info(IntEnum):
    CASE_SIMPLE = 0  # <- Case simple
    CASE_TOURELLE = 1  # <- Tourelle
    CASE_BASE = 2  # <- Base du joueur
    CASE_FONTAINE = 3  # <- Fontaine magique
    CASE_ARTEFACT = 4  # <- Artefact magique
    CASE_ERREUR = 5  # <- Erreur


# Erreurs possibles
class erreur(IntEnum):
    OK = 0  # <- L'action s'est effectuée avec succès
    ANNULER_IMPOSSIBLE = 1  # <- Aucune action à annuler
    CASE_IMPOSSIBLE = 2  # <- Cette case n'existe pas
    CASE_ADVERSE = 3  # <- Vous ne contrôlez pas cette case
    CASE_UTILISEE = 4  # <- Cette case n'est pas libre
    CASE_VIDE = 5  # <- Cette case est vide
    VALEUR_INVALIDE = 6  # <- Cette valeur est invalide
    MAGIE_INSUFFISANTE = 7  # <- Vous n'avez pas assez de magie
    SORCIERS_INSUFFISANTS = 8  # <- Vous n'avez pas assez de sorciers
    ATTAQUE_INSUFFISANTE = 9  # <- Vous n'avez pas assez de points d'attaque
    PHASE_INCORRECTE = 10  # <- Cette action ne peut pas être utilisée lors de cette phase du jeu.
    PORTEE_INSUFFISANTE = 11  # <- Vous n'avez pas assez de portée pour effectuer cette action
    PERDANT = 12  # <- Vous avez perdu et ne pouvez pas effectuer d'actions


from collections import namedtuple

# Représente la position sur la carte
position = namedtuple("position",
    'x ' # <- Coordonnée en X
    'y ' # <- Coordonnée en Y
)

# Représente une tourelle
tourelle = namedtuple("tourelle",
    'pos ' # <- Position de la tourelle
    'portee ' # <- Portée de la tourelle
    'joueur ' # <- Joueur qui possède la tourelle
    'vie ' # <- Nombre de points de vie de la tourelle
    'attaque ' # <- Nombre de points d'attaque de la tourelle
)


