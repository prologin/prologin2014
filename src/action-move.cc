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

#include "action-move.hh"

ActionMove::ActionMove(position start, position dest, int nb_wizards, int player)
    : start_(start)
    , dest_(dest)
    , nb_wizards_(nb_wizards)
    , player_id_(player)
{
}

ActionMove::ActionMove()
    : start_({ -1, -1 })
    , dest_({ -1, -1 })
    , nb_wizards_(-1)
    , player_id_(-1)
{
}

int ActionMove::check(const GameState* st) const
{
    if (st->getPhase() != PHASE_MOVE)
        return PHASE_INCORRECTE;

    const Cell* cell_start = st->get_map()->get_cell(start_);
    const Cell* cell_dest = st->get_map()->get_cell(dest_);

    int nb_movable = cell_start->get_nb_wizards_movable(player_id_);

    if (nb_wizards_ < 0)
        return VALEUR_INVALIDE;

    if (cell_start->get_type() == CASE_TOURELLE)
        return CASE_UTILISEE;

    if (cell_dest->get_type() == CASE_TOURELLE)
        return CASE_UTILISEE;

    if (nb_movable < nb_wizards_)
        return SORCIERS_INSUFFISANTS;

    if (start_.x < 0 || start_.x >= TAILLE_TERRAIN
        || start_.y < 0 || start_.y >= TAILLE_TERRAIN)
        return CASE_IMPOSSIBLE;

    if (dest_.x < 0 || dest_.x >= TAILLE_TERRAIN
        || dest_.y < 0 || dest_.y >= TAILLE_TERRAIN)
        return CASE_IMPOSSIBLE;

    if (start_ == dest_)
        return OK;

    if (distance(start_, dest_) > PORTEE_SORCIER)
        return CASE_IMPOSSIBLE;

    std::vector<position> path = st->get_map()->path(start_, dest_);

    // Checks if the cell is not too far from the wizard
    if (path.size() > PORTEE_SORCIER || path.size() == 0)
        return CASE_IMPOSSIBLE;

    return OK;
}

void ActionMove::handle_buffer(utils::Buffer& buf)
{
    buf.handle(start_);
    buf.handle(dest_);
    buf.handle(nb_wizards_);
    buf.handle(player_id_);
}

void ActionMove::apply_on(GameState* gameState) const
{
    Cell* cell_start = gameState->get_map()->get_cell(start_);
    Cell* cell_dest = gameState->get_map()->get_cell(dest_);


    cell_start->set_wizards(player_id_,
                            cell_start->get_nb_wizards(player_id_)
                            - nb_wizards_);
    cell_start->set_wizards_movable(player_id_,
                                    cell_start->
                                    get_nb_wizards_movable(player_id_)
                                    - nb_wizards_);
    cell_dest->set_wizards(player_id_,
                            cell_dest->get_nb_wizards(player_id_)
                            + nb_wizards_);
}
