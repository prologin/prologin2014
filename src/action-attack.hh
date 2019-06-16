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

#ifndef ACTION_ATTACK_HH
#define ACTION_ATTACK_HH

#include <rules/action.hh>

#include "constant.hh"
#include "game-state.hh"

class ActionAttack : public rules::Action<GameState>
{
public:
    ActionAttack(position pos, position target, int nb_wizards, int player);
    ActionAttack();

    virtual int check(const GameState* st) const;
    virtual void handle_buffer(utils::Buffer& buf);
    virtual void apply_on(GameState* st) const;

    uint32_t player_id() const { return player_id_; }
    uint32_t id() const { return ID_ACTION_ATTACK; }

private:
    position pos_;
    position target_;
    int nb_wizards_;
    int player_id_;
};

#endif // !ACTION_ATTACK_HH
