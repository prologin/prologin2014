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

#include "action-attack.hh"

ActionAttack::ActionAttack(position pos, position target, int nb_wizards, int player)
    : pos_(pos)
    , target_(target)
    , nb_wizards_(nb_wizards)
    , player_id_(player)
{
}

ActionAttack::ActionAttack()
    : pos_({-1, -1})
    , target_({ -1, -1 })
    , nb_wizards_(0)
    , player_id_(-1)
{
}

int ActionAttack::check(const GameState* st) const
{
    const Cell* cell_tower;
    const Cell* cell_init;

    if (st->has_lost(player_id_))
        return PERDANT;

    if (st->getPhase() != PHASE_SIEGE)
        return PHASE_INCORRECTE;

    if (!(cell_init = st->get_map()->get_cell(pos_)))
        return CASE_IMPOSSIBLE;

    if (!(cell_tower = st->get_map()->get_cell(target_)))
        return CASE_IMPOSSIBLE;

    if (nb_wizards_ < 0 || nb_wizards_
        > cell_init->get_nb_wizards(player_id_) - cell_init->get_tower_fighters())
        return SORCIERS_INSUFFISANTS;

    if (cell_init->get_nb_wizards(player_id_) <= 0)
        return SORCIERS_INSUFFISANTS;

    if (cell_init->get_type() == CASE_TOURELLE)
        return CASE_UTILISEE;

    if (cell_tower->get_type() != CASE_TOURELLE)
        return CASE_VIDE;


    if (distance(pos_, target_) > 1)
        return VALEUR_INVALIDE;

    return OK;
}

void ActionAttack::handle_buffer(utils::Buffer& buf)
{
    buf.handle(pos_);
    buf.handle(target_);
    buf.handle(nb_wizards_);
    buf.handle(player_id_);
}

void ActionAttack::apply_on(GameState* st) const
{
    Cell* cell_init = st->get_map()->get_cell(pos_);

    st->get_map()->get_cell(target_)->tower_attacked(nb_wizards_);

    cell_init->set_tower_fighters(cell_init->get_tower_fighters() - nb_wizards_);
}
