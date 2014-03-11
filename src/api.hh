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

#ifndef API_HH_
# define API_HH_

# include <vector>
# include <rules/game-state.hh>
# include <rules/player.hh>

# include "constant.hh"

/*!
** Method of this call are called by the candidat, throught 'interface.cc'
*/
class Api
{
    public:
        Api(rules::GameState* game_state, rules::Player_sptr player);
        virtual ~Api() { }

    private:
        rules::GameState* game_state_;
        rules::Player_sptr player_;

    public:

///
// Retourne le type de la case à l'emplacement `pos`
//
   case_info info_case(position pos);
///
// Retourne la liste des positions des tourelles qui appartiennent au joueur ``joueur``
//
   std::vector<position> tourelles_joueur(int joueur);
///
// Retourne la magie que possède le joueur ``joueur``
//
   int magie(int joueur);
///
// Retourne le nombre de sorciers sur la case ``pos``
//
   int nb_sorciers(position pos);
///
// Retourne le numéro du joueur qui contrôle la case ``pos``
//
   int joueur_case(position pos);
///
// Retourne la tourelle située sur la case ``pos``
//
   tourelle tourelle_case(position pos);
///
// Construire une tourelle à la position ``pos``
//
   erreur construire(position pos, int portee);
///
// Supprimer une tourelle à la position ``pos``
//
   erreur supprimer(position pos);
///
// Tirer avec ``pts`` points de dégats depuis la tourelles ``tourelle`` sur la position ``cible``
//
   erreur tirer(int pts, position tourelle, position cible);
///
// Créer ``nb`` sorciers dans la base
//
   erreur creer(int nb);
///
// Déplace ``nb`` sorciers de la position ``depart`` jusqu'à la position ``arrivee``.
//
   erreur deplacer(position depart, position arrivee, int nb);
///
// Attaquer la tourelle à la position ``cible`` depuis la position ``pos``
//
   erreur attaquer(position pos, position cible);
///
// Retourne le numéro de votre joueur
//
   int moi();
///
// Retourne la liste des numéros de vos adversaires
//
   std::vector<int> adversaires();
///
// Retourne le numéro du tour actuel
//
   int tour_actuel();
///
// Retourne la distance entre deux positions
//
   int distance(position depart, position arrivee);
///
// Annule la dernière action
//
   erreur annuler();
///
// Affiche le contenu d'une valeur de type case_info
//

///
// Affiche le contenu d'une valeur de type erreur
//

///
// Affiche le contenu d'une valeur de type position
//

///
// Affiche le contenu d'une valeur de type tourelle
//

};


#endif /* !API_HH_ */