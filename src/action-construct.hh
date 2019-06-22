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

#ifndef ACTION_CONSTRUCT_HH_
#define ACTION_CONSTRUCT_HH_

#include <rules/action.hh>

#include "constant.hh"
#include "game-state.hh"

class ActionConstruct : public rules::Action<GameState>
{
public:
    ActionConstruct(position pos, int range, int player);
    ActionConstruct();

    int check(const GameState& st) const override;
    void handle_buffer(utils::Buffer& buf) override;
    void apply_on(GameState* gameState) const override;

    uint32_t player_id() const override { return player_id_; }
    uint32_t id() const override { return ID_ACTION_CONSTRUCT; }

    int cost() const;

private:
    position pos_;
    int range_;
    int player_id_;
};

#endif /* !ACTION_CONSTRUCT_HH_ */
