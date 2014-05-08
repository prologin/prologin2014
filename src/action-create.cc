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

# include "action-create.hh"

ActionCreate::ActionCreate(int nb_wizards, int player)
    : nb_wizards_(nb_wizards)
    , player_id_(player)
{
}

ActionCreate::ActionCreate()
    : nb_wizards_(-1)
    , player_id_(-1)
{
}

int ActionCreate::check(const GameState* st) const
{
    if (st->has_lost(player_id_))
        return PERDANT;

    if (st->getPhase() != PHASE_CONSTRUCTION)
        return PHASE_INCORRECTE;

    if (nb_wizards_ <= 0)
        return VALEUR_INVALIDE;

    if (nbWizards >= 500000000 || st->get_magic(player_id_) < COUT_SORCIER * nb_wizards_)
        return MAGIE_INSUFFISANTE;

    return OK;
}

void ActionCreate::handle_buffer(utils::Buffer& buf)
{
    buf.handle(nb_wizards_);
    buf.handle(player_id_);
}

void ActionCreate::apply_on(GameState* gameState) const
{
    Cell* base = gameState->get_map()->get_cell(gameState->get_base(player_id_));
    base->set_wizards(player_id_,
                      base->get_nb_wizards(player_id_) + nb_wizards_);
    base->set_wizards_movable(player_id_,
                              base->get_nb_wizards_movable(player_id_) + nb_wizards_);
    gameState->set_magic(player_id_, gameState->get_magic(player_id_)
                         - COUT_SORCIER * nb_wizards_);
}
