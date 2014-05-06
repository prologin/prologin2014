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

# include "action-delete.hh"

ActionDelete::ActionDelete(position pos, int player)
    : position_(pos)
    , player_id_(player)
{
}

ActionDelete::ActionDelete()
    : position_({ -1, -1 })
    , player_id_(-1)
{
}

int ActionDelete::check(const GameState* st) const
{
    if (st->has_lost(player_id_))
        return PERDANT;

    if (st->getPhase() != PHASE_CONSTRUCTION)
        return PHASE_INCORRECTE;

    const Cell* cell = st->get_map()->get_cell(position_);

    if (position_.x < 0 || position_.x >= TAILLE_TERRAIN
        || position_.y < 0 || position_.y >= TAILLE_TERRAIN)
        return CASE_IMPOSSIBLE;

    if (cell->get_type() != CASE_TOURELLE)
        return CASE_VIDE;

    if (cell->get_player() != player_id_)
        return CASE_ADVERSE;

    return OK;
}

void ActionDelete::handle_buffer(utils::Buffer& buf)
{
    buf.handle(position_);
    buf.handle(player_id_);
}

void ActionDelete::apply_on(GameState* gameState) const
{
    Cell* cell = gameState->get_map()->get_cell(position_);
    cell->delete_tower();

    // Magic gained from the supression
    gameState->set_magic(player_id_,
                         gameState->get_magic(player_id_) + MAGIE_SUPPRESSION);
}
