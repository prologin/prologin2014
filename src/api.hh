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

# include <rules/game-state.hh>
# include <rules/player.hh>
# include <rules/actions.hh>

# include <stdlib.h>
# include <cstdlib>
# include <vector>

# include "constant.hh"
# include "cell.hh"
# include "position.hh"
# include "game-state.hh"
# include "action-create.hh"
# include "action-delete.hh"
# include "action-move.hh"
# include "action-shoot.hh"
# include "action-construct.hh"
# include "action-attack.hh"

/*!
** Method of this call are called by the candidat, throught 'interface.cc'
*/
class Api
{
    public:
        Api(GameState* game_state, rules::Player_sptr player);
        virtual ~Api() { }

        const rules::Player_sptr player() const { return player_; }
        void player_set(rules::Player_sptr player) { player_ = player; }
        rules::Actions* actions() { return &actions_; }

        const GameState* game_state() const { return game_state_; }
        GameState* game_state() { return game_state_; }
        void game_state_set(rules::GameState* gs)
            { game_state_ = dynamic_cast<GameState*>(gs); }

    private:
        GameState* game_state_;
        rules::Player_sptr player_;
        rules::Actions actions_;

    public:

///
// Retourne le type de la case à l'emplacement `pos`
//
   case_info info_case(position pos);
///
// Retourne la liste des tourelles qui appartiennent au joueur ``joueur``
//
   std::vector<tourelle> tourelles_joueur(int joueur);
///
// Retourne la magie que possède le joueur ``joueur``
//
   int magie(int joueur);
///
// Retourne le nombre de sorciers du joueur ``joueur`` sur la case ``pos``
//
   int nb_sorciers(position pos, int joueur);
///
// Retourne le nombre de sorciers du joueur ``joueur`` déplacables sur la case ``pos``
//
   int nb_sorciers_deplacables(position pos, int joueur);
///
// Retourne le numéro du joueur qui contrôle la case ``pos``
//
   int joueur_case(position pos);
///
// Retourne la tourelle située sur la case ``pos``
//
   tourelle tourelle_case(position pos);
///
// Retourne la position de la base du joueur ``joueur``
//
   position base_joueur(int joueur);
///
// Retourne vrai si l'on peut construire sur la case ``pos``
//
   bool constructible(position pos, int joueur);
///
// Retourne la liste des positions constituant le plus court chemin allant de la case ``pos1`` à la case ``pos2``. Attention : Cette fonction est lente.
//
   std::vector<position> chemin(position pos1, position pos2);
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
