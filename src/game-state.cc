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

GameState::GameState(Map* map, rules::Players_sptr players)
    : rules::GameState(),
      map_(map),
      players_(players),
      current_turn_(0)
{
    game_phase_ = PHASE_CONSTRUCTION;

    // list of the positions of bases
    std::vector<position> list_base =
    {
        { 0, 0 },
        { TAILLE_TERRAIN - 1, 0 },
        { TAILLE_TERRAIN - 1, TAILLE_TERRAIN - 1 },
        { 0, TAILLE_TERRAIN - 1 },
    };

    int i = 0;

    for (auto& p : players_->players)
    {
        if (p->type == rules::PLAYER)
        {
            players_ids_[p->id] = p;
            magic_[p->id] = 0;
            bases_players_[p->id] = list_base[i];
            map_->get_cell(list_base[i])->set_player(p->id);
            i++;
        }
    }
}

GameState::GameState(const GameState& st)
    : rules::GameState(st)
    , map_(new Map(*st.map_))
    , players_(st.players_)
    , losers_(st.losers_)
    , current_turn_(st.current_turn_)
    , game_phase_(st.game_phase_)
{
     players_ids_.insert(st.players_ids_.begin(), st.players_ids_.end());
     magic_.insert(st.magic_.begin(), st.magic_.end());
     bases_players_.insert(st.bases_players_.begin(), st.bases_players_.end());
}

rules::GameState* GameState::copy() const
{
    return new GameState(*this);
}

std::vector<int> GameState::get_opponents(int player_id) const
{
    std::vector<int> opponents;
    for (auto i : players_->players)
        if ((int) i->id != player_id)
            opponents.push_back(i->id);
    return opponents;
}

position GameState::get_base(int player) const
{
    return bases_players_.find(player)->second;
}

void GameState::increment_turn()
{
    current_turn_++;
}

int GameState::get_current_turn() const
{
    return current_turn_;
}

std::vector<tourelle> GameState::get_towers(int player)
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
    return map_->resolve_fights();
}

void GameState::resolve_wizard_movable()
{
    return map_->resolve_wizard_movable();
}

void GameState::resolve_tower_magic()
{
    return map_->resolve_tower_magic();
}

GameState::~GameState()
{
}

void GameState::setPhase(game_phase phase)
{
    game_phase_ = phase;
}

game_phase GameState::getPhase() const
{
    return game_phase_;
}

void GameState::check_losers()
{
    int owner_base = -1;
    for (auto& p : players_ids_)
    {
        owner_base = map_->get_cell(bases_players_[p.first])->get_player();
        if (owner_base != p.first)
        {
            // update score of the winner
            if (losers_.find(p.first) == losers_.end())
            {
                players_ids_[owner_base]->score += POINTS_VAINQUEUR;
                losers_.emplace(p.first);
                // delete all his wizards and all his towers
                map_->delete_all(p.first);
            }
        }
    }
}

bool GameState::has_lost(int player) const
{
    return (losers_.find(player) != losers_.end());
}
