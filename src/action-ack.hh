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

#ifndef ACTION_ACK_HH_

#define ACTION_ACK_HH_

#include <rules/action.hh>

#include "constant.hh"
#include "game-state.hh"

class ActionAck : public rules::Action<GameState>
{
public:
    ActionAck(int player);

    ActionAck();

    virtual int check(const GameState&) const override { return 0; }
    virtual void handle_buffer(utils::Buffer&) override;

    uint32_t player_id() const override { return player_; }
    uint32_t id() const override { return ID_ACTION_ACK; }

protected:
    virtual void apply_on(GameState*) const override {}

protected:
    int player_;
    int id_;
};

#endif /* !ACTION_ACK_HH_ */
