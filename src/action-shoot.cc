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

# include "action-shoot.hh"

ActionShoot::ActionShoot(int        points,
                         position   tower,
                         position   target,
                         int        player)
    : points_(points)
    , tower_(tower)
    , target_(target)
    , player_id_(player)
{
}

ActionShoot::ActionShoot()
    : points_(-1)
    , tower_({ -1, -1 })
    , target_({ -1, -1 })
    , player_id_(-1)
{
}

int ActionShoot::check(const GameState* st) const
{
    if (st->has_lost(player_id_))
        return PERDANT;

    if (st->getPhase() != PHASE_SHOOT)
        return PHASE_INCORRECTE;

    const Cell* cell_tower = st->get_map()->get_cell(tower_);

    if (tower_.x < 0 || tower_.x >= TAILLE_TERRAIN
        || tower_.y < 0 || tower_.y >= TAILLE_TERRAIN)
        return CASE_IMPOSSIBLE;

    if (target_.x < 0 || target_.x >= TAILLE_TERRAIN
        || target_.y < 0 || target_.y >= TAILLE_TERRAIN)
        return CASE_IMPOSSIBLE;

    if (points_ < 0)
        return VALEUR_INVALIDE;

    if (cell_tower->get_type() != CASE_TOURELLE)
        return CASE_VIDE;

    // Check of the tower
    tourelle tower = cell_tower->get_tower();
    int dist = distance(tower_, target_);

    if (dist > tower.portee)
        return VALEUR_INVALIDE;

    if (cell_tower->get_player() != player_id_)
        return CASE_ADVERSE;

    if (tower.attaque < points_)
        return ATTAQUE_INSUFFISANTE;

    return OK;
}

void ActionShoot::handle_buffer(utils::Buffer& buf)
{
    buf.handle(points_);
    buf.handle(tower_);
    buf.handle(target_);
    buf.handle(player_id_);
}

void ActionShoot::apply_on(GameState* gameState) const
{
    Cell* cell_target = gameState->get_map()->get_cell(target_);
    Cell* cell_tower = gameState->get_map()->get_cell(tower_);

    cell_target->wizards_attacked(points_, player_id_);

    tourelle t = cell_tower->get_tower();
    cell_tower->set_magic_tower(t.attaque - points_);
}
