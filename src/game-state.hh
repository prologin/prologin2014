#include "game-state.hh"

GameState::GameState(rules::Players_sptr players)
    : rules::GameState(),
      players_(players)
{

}
