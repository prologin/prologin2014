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

#include "api.hh"
#include "dumper.hh"

// global used in interface.cc
Api* api;

Api::Api(GameState* game_state, rules::Player_sptr player)
    : game_state_(game_state),
      player_(player)
{
  api = this;
}

///
// Retourne le type de la case à l'emplacement `pos`
//
case_info Api::info_case(position pos)
{
  Cell *c = game_state_->get_map()->get_cell(pos);
  if (c)
      return c->get_type();

  return CASE_ERREUR;
}

///
// Retourne la liste des tourelles qui appartiennent au joueur ``joueur``
//
std::vector<tourelle> Api::tourelles_joueur(int joueur)
{
    return game_state_->get_towers(joueur);
}

///
// Retourne la magie que possède le joueur ``joueur``
//
int Api::magie(int joueur)
{
    return game_state_->get_magic(joueur);
}

///
// Retourne le nombre de sorciers du joueur ``joueur`` sur la case ``pos``
//
int Api::nb_sorciers(position pos, int joueur)
{
  Cell* c = game_state_->get_map()->get_cell(pos);

  if (!c)
      return -1;

  return c->get_nb_wizards(joueur);
}

///
// Retourne le nombre de sorciers du joueur ``joueur`` déplacables sur la case ``pos``
//
int Api::nb_sorciers_deplacables(position pos, int joueur)
{
  Cell *c = game_state_->get_map()->get_cell(pos);

  if (!c)
      return -1;

  return c->get_nb_wizards_movable(joueur);
}

///
// Retourne le numéro du joueur qui contrôle la case ``pos``
//
int Api::joueur_case(position pos)
{
  Cell* c = game_state_->get_map()->get_cell(pos);
  if (c)
      return c->get_player();
  return  -1;
}

///
// Retourne la tourelle située sur la case ``pos``
//
tourelle Api::tourelle_case(position pos)
{
  Cell* c = game_state_->get_map()->get_cell(pos);

  if (!c)
      return { { -1, -1 }, 0, -1, 0, 0 };

  tourelle t = c->get_tower();

  return t;
}

///
// Retourne la position de la base du joueur ``joueur``
//
position Api::base_joueur(int joueur)
{
    return game_state_->get_base(joueur);
}

///
// Retourne vrai si l'on peut construire sur la case ``pos``
//
bool Api::constructible(position pos, int joueur)
{
    return game_state_->get_map()->buildable(pos, joueur);
}

///
// Retourne la liste des positions constituant le plus court chemin allant de
// la case ``pos1`` à la case ``pos2``. Attention : Cette fonction est lente.
//
std::vector<position> Api::chemin(position pos1, position pos2)
{
    return game_state_->get_map()->path(pos1, pos2);
}

///
// Construire une tourelle à la position ``pos``
//
erreur Api::construire(position pos, int portee)
{
    rules::IAction_sptr action(new ActionConstruct(pos, portee, player_->id));

    erreur err;

    if ((err = static_cast<erreur>(action->check(game_state_))) != OK)
        return err;

    actions_.add(action);
    game_state_set(action->apply(game_state()));

    return OK;
}

///
// Supprimer une tourelle à la position ``pos``
//
erreur Api::supprimer(position pos)
{
    rules::IAction_sptr action(new ActionDelete(pos, player_->id));

    erreur err;

    if ((err = static_cast<erreur>(action->check(game_state_))) != OK)
        return err;

    actions_.add(action);
    game_state_set(action->apply(game_state()));

    return OK;
}

///
// Tirer avec ``pts`` points de dégats depuis la tourelles ``tourelle`` sur la
// position ``cible``
//
erreur Api::tirer(int pts, position tourelle, position cible)
{
    rules::IAction_sptr action(new ActionShoot(pts,
                                               tourelle,
                                               cible,
                                               player_->id));

    erreur err;

    if ((err = static_cast<erreur>(action->check(game_state_))) != OK)
        return err;

    actions_.add(action);
    game_state_set(action->apply(game_state()));

    return OK;
}

///
// Créer ``nb`` sorciers dans la base
//
erreur Api::creer(int nb)
{
    rules::IAction_sptr action(new ActionCreate(nb, player_->id));

    erreur err;

    if ((err = static_cast<erreur>(action->check(game_state_))) != OK)
        return err;

    actions_.add(action);
    game_state_set(action->apply(game_state()));

    return OK;
}

///
// Déplace ``nb`` sorciers de la position ``depart`` jusqu'à la position
// ``arrivee``.
//
erreur Api::deplacer(position depart, position arrivee, int nb)
{
    rules::IAction_sptr action(new ActionMove(depart,
                                              arrivee,
                                              nb,
                                              player_->id));

    erreur err;

    if ((err = static_cast<erreur>(action->check(game_state_))) != OK)
        return err;

    actions_.add(action);
    game_state_set(action->apply(game_state()));

    return OK;
}

///
// Attaquer la tourelle à la position ``cible`` depuis la position ``pos``
//
erreur Api::assieger(position pos, position cible)
{
    rules::IAction_sptr action(new ActionAttack(pos, cible, player_->id));

    erreur err;

    if ((err = static_cast<erreur>(action->check(game_state_))) != OK)
        return err;

    actions_.add(action);
    game_state_set(action->apply(game_state()));

    return OK;
}

///
// Retourne le numéro de votre joueur
//
int Api::moi()
{
    return player_->id;
}

///
// Retourne la liste des numéros de vos adversaires
//
std::vector<int> Api::adversaires()
{
    return game_state_->get_opponents(player_->id);
}

///
// Retourne le numéro du tour actuel
//
int Api::tour_actuel()
{
    return game_state_->get_current_turn();
}

// Retourne la distance entre deux positions
//
int Api::distance(position depart, position arrivee)
{
    // function of position.hh
    return distance(depart, arrivee);
}
///
///
// Annule la dernière action
//
erreur Api::annuler()
{
    if (!game_state_->can_cancel())
        return ANNULER_IMPOSSIBLE;

    game_state_ = rules::cancel(game_state_);

    return OK;
}


///
// Return un dump JSON de l’état complet du jeu.
//
char* Api::get_dump()
{
    if (player_->type != rules::SPECTATOR)
        return NULL;
    return dump_game_state(*game_state_, actions_);
}
