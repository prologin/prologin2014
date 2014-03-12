#include "game-state.hh"

GameState::GameState(rules::Players_sptr players)
    : rules::GameState(),
      players_(players)
{

    for (auto& p : player_->players)
        if (p->type == rules::PLAYER)
        {
            players_ids_[p->id] = p;
        }
}

GameState::GameState(const GameState& st)
    : rules::GameState(st),
      players_(players)
{
     player_ids_.insert(st.player_ids_.begin(), st.player_ids_.end());
}

rules::GameState* copy() const
{
    return new GameState(*this);
}


GameState::~GameState()
{
}
