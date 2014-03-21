#include "game-state.hh"

GameState::GameState(Map* map, rules::Players_sptr players)
    : rules::GameState(),
      map_(map),
      players_(players),
      current_turn_(0)
{

    //for (auto& p : players_->players)
    //    if (p->type == rules::PLAYER)
    //    {
    //        players_ids_[p->id] = p;
    //    }
}

GameState::GameState(const GameState& st)
    : rules::GameState(st)
{
     players_ids_.insert(st.players_ids_.begin(), st.players_ids_.end());
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

int GameState::get_current_turn() const
{
    return current_turn_;
}

void GameState::increment_turn()
{
    current_turn_++;
}

Map* GameState::get_map() const
{
    return map_;
}

bool GameState::add_tower(position pos, int player)
{
    tourelle tower = { pos, PORTEE_TOURELLE, player, VIE_TOURELLE, MAGIE_TOUR };
    towers_.push_back(tower);
    return map_->get_cell(pos)->put_tower(tower);
}

//std::vector<tourelle> GameState::get_tourelles(int player)
//{
    //std::vector<tourelle> towers;
    //return 
//}

GameState::~GameState()
{
}
