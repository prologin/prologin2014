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

#include "game-state.hh"

GameState::GameState(Map* map, const rules::Players& players)
    : rules::GameState(), map_(map), players_(players), current_round_(0)
{
    game_phase_ = PHASE_CONSTRUCTION;

    // list of the positions of bases
    std::vector<position> list_base = {
        {0, 0},
        {TAILLE_TERRAIN - 1, 0},
        {TAILLE_TERRAIN - 1, TAILLE_TERRAIN - 1},
        {0, TAILLE_TERRAIN - 1},
    };

    int i = 0;

    for (auto& player : players_)
    {
        if (player->type == rules::PLAYER)
        {
            players_ids_[player->id] = player;
            magic_[player->id] =
                MAGIE_TOUR; // Initial amount for the first round
            bases_players_[player->id] = list_base[i];
            map_->get_cell(list_base[i])->set_player(player->id);
            i++;
        }
    }
}

GameState::GameState(const GameState& st)
    : rules::GameState(st)
    , map_(new Map(*st.map_))
    , players_(st.players_)
    , losers_(st.losers_)
    , current_round_(st.current_round_)
    , game_phase_(st.game_phase_)
{
    players_ids_.insert(st.players_ids_.begin(), st.players_ids_.end());
    magic_.insert(st.magic_.begin(), st.magic_.end());
    bases_players_.insert(st.bases_players_.begin(), st.bases_players_.end());
}

GameState* GameState::copy() const
{
    return new GameState(*this);
}

std::vector<int> GameState::get_opponents(int player_id) const
{
    std::vector<int> opponents;
    for (auto player : players_)
        if (static_cast<int>(player->id) != player_id && !has_lost(player->id))
            opponents.push_back(player->id);
    return opponents;
}

position GameState::get_base(int player) const
{
    return bases_players_.find(player)->second;
}

void GameState::increment_round()
{
    current_round_++;
}

int GameState::get_current_round() const
{
    return current_round_;
}

std::vector<tourelle> GameState::get_towers(int player) const
{
    return map_->get_towers(player);
}

int GameState::get_magic(int player) const
{
    return magic_.find(player)->second;
}

void GameState::set_magic(int player, int magic)
{
    magic_.find(player)->second = magic;
}

const Map* GameState::get_map() const
{
    return map_;
}

Map* GameState::get_map()
{
    return map_;
}

int GameState::get_player_artefact()
{
    return map_->get_player_artefact();
}

int GameState::get_nb_fontains(int player_id)
{
    return map_->get_nb_fontains(player_id);
}

void GameState::resolve_fights()
{
    return map_->resolve_fights(magic_);
}

void GameState::resolve_wizard_movable()
{
    return map_->resolve_wizard_movable();
}

void GameState::resolve_tower_magic()
{
    return map_->resolve_tower_magic();
}

void GameState::resolve_tower_fighters()
{
    return map_->resolve_tower_fighters();
}

GameState::~GameState()
{
    delete map_;
}

void GameState::setPhase(game_phase phase)
{
    game_phase_ = phase;
}

game_phase GameState::getPhase() const
{
    return game_phase_;
}

void GameState::resolve_base_released()
{
    int taken;

    for (auto& p : players_ids_)
    {
        if (!has_lost(p.first))
        {
            taken = map_->get_cell(get_base(p.first))->get_taken();
            if (taken != -1 &&
                !map_->get_cell(get_base(p.first))->get_nb_wizards_total())
                map_->get_cell(get_base(p.first))->set_taken(-1);
        }
    }
}

void GameState::check_losers()
{
    int owner_base = -1;
    std::vector<int> losers_this_round;

    for (auto& p : players_ids_)
    {
        owner_base = map_->get_cell(bases_players_[p.first])->get_taken();
        // If owner_base == -1, it means that the player already lost
        if (owner_base != -1)
        {
            // update score of the winner
            if (losers_.find(p.first) == losers_.end())
            {
                players_ids_[owner_base]->score += POINTS_VAINQUEUR;
                // We cannot declare this user as loser before the end of the
                // function, in case  players kill each other at the same time.
                losers_this_round.push_back(p.first);
                // map_->get_cell(bases_players_[p.first])->set_taken(-1);
            }
        }
    }

    // We declare the losers as losers
    for (int x : losers_this_round)
    {
        // delete all his wizards and all his towers
        map_->delete_all(x);
        magic_[x] = 0;
        losers_.insert(x);
        // map_->get_cell(bases_players_[x])->set_player(-1);
    }
}

bool GameState::has_lost(int player) const
{
    return (losers_.find(player) != losers_.end());
}
