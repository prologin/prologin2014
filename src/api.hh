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

#ifndef API_HH_
#define API_HH_

#include <cstdlib>
#include <stdlib.h>
#include <vector>

#include <rules/actions.hh>
#include <rules/api.hh>
#include <rules/game-state.hh>
#include <rules/player.hh>

#include "action-attack.hh"
#include "action-construct.hh"
#include "action-create.hh"
#include "action-delete.hh"
#include "action-move.hh"
#include "action-shoot.hh"
#include "cell.hh"
#include "constant.hh"
#include "game-state.hh"
#include "position.hh"

/*!
** Method of this call are called by the candidat, throught 'interface.cc'
*/
class Api final : public rules::Api<GameState, erreur>
{
public:
    Api(std::unique_ptr<GameState> game_state, rules::Player_sptr player);
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
    // Retourne le nombre de sorciers du joueur ``joueur`` déplacables sur la
    // case ``pos``
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
    // Retourne la liste des positions constituant le plus court chemin allant
    // de la case ``pos1`` à la case ``pos2``. Attention : Cette fonction est
    // lente.
    //
    std::vector<position> chemin(position pos1, position pos2);
    ///
    // Construire une tourelle à la position ``pos``
    //
    ApiActionFunc<ActionConstruct, position> construire{this};
    ///
    // Supprimer une tourelle à la position ``pos``
    //
    ApiActionFunc<ActionDelete, position> supprimer{this};
    ///
    // Tirer avec ``pts`` points de dégats depuis la tourelle sur ``pos`` sur
    // la position ``cible``
    //
    ApiActionFunc<ActionShoot> tirer{this};
    ///
    // Créer ``nb`` sorciers dans la base
    //
    ApiActionFunc<ActionCreate> creer{this};
    ///
    // Déplace ``nb`` sorciers de la position ``depart`` jusqu'à la position
    // ``arrivee``.
    //
    ApiActionFunc<ActionMove, position> deplacer{this};
    ///
    // Attaquer la tourelle à la position ``cible`` depuis la position ``pos``
    //
    ApiActionFunc<ActionAttack, position> assieger{this};
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
    // Return un dump JSON de l’état complet du jeu.
    //
    char* get_dump();
};

#endif /* !API_HH_ */
