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

#ifndef GAME_STATE_HH
#define GAME_STATE_HH

#include "map.hh"

#include <rules/game-state.hh>
#include <rules/player.hh>

#include <unordered_map>
#include <unordered_set>

enum game_phase
{
    PHASE_CONSTRUCTION,
    PHASE_MOVE,
    PHASE_SHOOT,
    PHASE_SIEGE
};

enum action_id
{
    ID_ACTION_CONSTRUCT,
    ID_ACTION_DELETE,
    ID_ACTION_SHOOT,
    ID_ACTION_CREATE,
    ID_ACTION_MOVE,
    ID_ACTION_ATTACK,
    ID_ACTION_ERASE,
    ID_ACTION_ACK
};

class GameState : public rules::GameState
{
public:
    GameState(Map* map, rules::Players_sptr players);
    GameState(const GameState& st);
    virtual rules::GameState* copy() const;
    ~GameState();

    /* round handling */
    void increment_round();
    int get_current_round() const;

    /* Towers */
    bool add_tower(position pos, int player);
    std::vector<tourelle> get_towers(int player) const;

    /* Magic */
    int get_magic(int player) const;
    void set_magic(int player, int magic);

    std::vector<int> get_opponents(int player) const;
    position get_base(int player) const;

    const Map* get_map() const;
    Map* get_map();

    rules::Players_sptr get_players() const { return players_; }

    std::map<int, rules::Player_sptr> get_players_ids() const
    {
        return players_ids_;
    }

    std::unordered_set<int> get_losers_ids() const { return losers_; }

    void setPhase(game_phase phase);
    game_phase getPhase() const;

    // Function for the score
    int get_player_artefact();
    int get_nb_fontains(int player_id);

    // Resolve everything
    void resolve_fights();
    void resolve_wizard_movable();
    void resolve_tower_magic();
    void resolve_tower_fighters();

    void check_losers();
    void resolve_base_released();
    bool has_lost(int player) const;

private:
    Map* map_;
    rules::Players_sptr players_;
    std::map<int, rules::Player_sptr> players_ids_;
    std::unordered_set<int> losers_;
    int current_round_;

    std::map<int, int> magic_;
    std::map<int, position> bases_players_;

    game_phase game_phase_;
};

#endif /* !GAME_STATE_HH */
