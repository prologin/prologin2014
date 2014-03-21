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

#include <stdlib.h>

#include "api.hh"

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
  // FIXME : Should be CASE_ERROR (we have to add it to the yml file)
  return CASE_SIMPLE;
}

///
// Retourne la liste des tourelles qui appartiennent au joueur ``joueur``
//
std::vector<tourelle> Api::tourelles_joueur(int joueur)
{
  // TODO
  abort();
}

///
// Retourne la magie que possède le joueur ``joueur``
//
int Api::magie(int joueur)
{
  // TODO
  abort();
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

  // TODO : ERROR
  //if (!c)
  //    return NULL;
  tourelle t = c->get_tower();
  return t;
}

///
// Construire une tourelle à la position ``pos``
//
erreur Api::construire(position pos, int portee)
{
  // TODO
  abort();
}

///
// Supprimer une tourelle à la position ``pos``
//
erreur Api::supprimer(position pos)
{
  // TODO
  abort();
}

///
// Tirer avec ``pts`` points de dégats depuis la tourelles ``tourelle`` sur la position ``cible``
//
erreur Api::tirer(int pts, position tourelle, position cible)
{
  // TODO
  abort();
}

///
// Créer ``nb`` sorciers dans la base
//
erreur Api::creer(int nb)
{
  // TODO
  abort();
}

///
// Déplace ``nb`` sorciers de la position ``depart`` jusqu'à la position ``arrivee``.
//
erreur Api::deplacer(position depart, position arrivee, int nb)
{
  // TODO
  abort();
}

///
// Attaquer la tourelle à la position ``cible`` depuis la position ``pos``
//
erreur Api::attaquer(position pos, position cible)
{
  // TODO
  abort();
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

///
// Retourne la distance entre deux positions
//
int Api::distance(position depart, position arrivee)
{
    return abs(depart.x - arrivee.x) + abs(depart.y - arrivee.y);
}

///
// Annule la dernière action
//
erreur Api::annuler()
{
  // TODO
  abort();
}

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

