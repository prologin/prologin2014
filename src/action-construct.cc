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

#include <algorithm>
#include <cmath>

#include "action-construct.hh"

static const int PORTEE_TOURELLE_MAX = 7;
static const int MAX_NUMBER_TOURELLE = 9;

ActionConstruct::ActionConstruct(position pos, int range, int player)
    : pos_(pos)
    , range_(range)
    , player_id_(player)
{
}

ActionConstruct::ActionConstruct()
    : pos_({-1, -1})
    , range_(-1)
    , player_id_(-1)
{
}

int ActionConstruct::cost() const
{
    int c = COUT_TOURELLE;
    if (range_ > PORTEE_TOURELLE)
        c += COUT_PORTEE + (range_ - PORTEE_TOURELLE) * (range_ - PORTEE_TOURELLE);
    return c;
}

int ActionConstruct::check(const GameState* st) const
{
    if (st->has_lost(player_id_))
        return PERDANT;

    if (st->getPhase() != PHASE_CONSTRUCTION)
        return PHASE_INCORRECTE;

    const Cell* cell;

    if (!(cell = st->get_map()->get_cell(pos_)))
        return CASE_IMPOSSIBLE;

    if (cell->get_type() != CASE_SIMPLE || cell->get_nb_wizards_total())
        return CASE_UTILISEE;

    if (!st->get_map()->buildable(pos_, player_id_))
        return CASE_ADVERSE;

    if (range_ < PORTEE_TOURELLE)
        return VALEUR_INVALIDE;

    if (range_ > PORTEE_TOURELLE_MAX)
        return VALEUR_INVALIDE;

    if (st->get_towers(player_id_).size() + 1 > MAX_NUMBER_TOURELLE)
        return VALEUR_INVALIDE;

    if (st->get_magic(player_id_) < cost())
        return MAGIE_INSUFFISANTE;

    return OK;
}

void ActionConstruct::handle_buffer(utils::Buffer& buf)
{
    buf.handle(pos_);
    buf.handle(range_);
    buf.handle(player_id_);
}

void ActionConstruct::apply_on(GameState* st) const
{
    tourelle t =
    {
        {
            pos_.x,
            pos_.y
        },
        range_,
        player_id_,
        VIE_TOURELLE,
        ATTAQUE_TOURELLE
    };
    st->get_map()->get_cell(pos_)->put_tower(t);
    st->get_map()->add_constructing(pos_);
    st->set_magic(player_id_, st->get_magic(player_id_) - cost());
}
