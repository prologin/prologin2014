///
// This file has been generated, if you wish to
// modify it in a permanent way, please refer
// to the script file : gen/generator_python.rb
//

#ifndef INTERFACE_HH_
# define INTERFACE_HH_

# include <Python.h>
# include <vector>
# include <string>

///
// Information sur les cases
//
typedef enum case_info {
  CASE_SIMPLE, /* <- Case simple */
  CASE_TOURELLE, /* <- Tourelle */
  CASE_BASE, /* <- Base du joueur */
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
  PHASE_INCORRECTE, /* <- Cette action ne peut pas être utilisée lors de cette phase du jeu. */
  PORTEE_INSUFFISANTE, /* <- Vous n'avez pas assez de portée pour effectuer cette action */
  PERDANT, /* <- Vous avez perdu et ne pouvez pas effectuer d'actions */
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
  int attaque;  /* <- Nombre de points d'attaque de la tourelle */
} tourelle;



extern "C" {

///
// Retourne le type de la case à l'emplacement `pos`
//
case_info api_info_case(position pos);

///
// Retourne la liste des tourelles qui appartiennent au joueur ``joueur``
//
std::vector<tourelle> api_tourelles_joueur(int joueur);

///
// Retourne la magie que possède le joueur ``joueur``
//
int api_magie(int joueur);

///
// Retourne le nombre de sorciers du joueur ``joueur`` sur la case ``pos``
//
int api_nb_sorciers(position pos, int joueur);

///
// Retourne le nombre de sorciers du joueur ``joueur`` déplacables sur la case ``pos``
//
int api_nb_sorciers_deplacables(position pos, int joueur);

///
// Retourne le numéro du joueur qui contrôle la case ``pos``
//
int api_joueur_case(position pos);

///
// Retourne la tourelle située sur la case ``pos``
//
tourelle api_tourelle_case(position pos);

///
// Retourne la position de la base du joueur ``joueur``
//
position api_base_joueur(int joueur);

///
// Retourne vrai si l'on peut construire sur la case ``pos``
//
bool api_constructible(position pos, int joueur);

///
// Retourne la liste des positions constituant le plus court chemin allant de la case ``pos1`` à la case ``pos2``. Attention : Cette fonction est lente.
//
std::vector<position> api_chemin(position pos1, position pos2);

///
// Construire une tourelle à la position ``pos``
//
erreur api_construire(position pos, int portee);

///
// Supprimer une tourelle à la position ``pos``
//
erreur api_supprimer(position pos);

///
// Tirer avec ``pts`` points de dégats depuis la tourelles ``tourelle`` sur la position ``cible``
//
erreur api_tirer(int pts, position tourelle, position cible);

///
// Créer ``nb`` sorciers dans la base
//
erreur api_creer(int nb);

///
// Déplace ``nb`` sorciers de la position ``depart`` jusqu'à la position ``arrivee``.
//
erreur api_deplacer(position depart, position arrivee, int nb);

///
// Attaquer la tourelle à la position ``cible`` depuis la position ``pos``
//
erreur api_assieger(position pos, position cible, int nb_sorciers);

///
// Retourne le numéro de votre joueur
//
int api_moi();

///
// Retourne la liste des numéros de vos adversaires
//
std::vector<int> api_adversaires();

///
// Retourne le numéro du tour actuel
//
int api_tour_actuel();

///
// Retourne la distance entre deux positions
//
int api_distance(position depart, position arrivee);

///
// Annule la dernière action
//
erreur api_annuler();

///
// Affiche le contenu d'une valeur de type case_info
//
void api_afficher_case_info(case_info v);

///
// Affiche le contenu d'une valeur de type erreur
//
void api_afficher_erreur(erreur v);

///
// Affiche le contenu d'une valeur de type position
//
void api_afficher_position(position v);

///
// Affiche le contenu d'une valeur de type tourelle
//
void api_afficher_tourelle(tourelle v);

char* api_get_dump();

}

#endif // !INTERFACE_HH_
