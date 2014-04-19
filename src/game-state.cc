#include "game-state.hh"

GameState::GameState(Map* map, rules::Players_sptr players)
    : rules::GameState(),
      map_(map),
      players_(players),
      current_turn_(0)
{
    // TODO

    std::map<int, position> bases_players;

    // list of the positions of bases
    std::vector<position> list_base =
    {
        { 0, 0 },
        { TAILLE_TERRAIN - 1, 0 },
        { TAILLE_TERRAIN - 1, TAILLE_TERRAIN - 1 },
        { 0, TAILLE_TERRAIN - 1 },
    };

    // list of the positions of fontains
    std::vector<position> list_fontains =
    {
        { 0, TAILLE_TERRAIN / 2 },
        { TAILLE_TERRAIN, TAILLE_TERRAIN / 2 },
        { TAILLE_TERRAIN / 2, 0 },
        { TAILLE_TERRAIN / 2, TAILLE_TERRAIN },
    };

    // list of the positions of artefacts
    std::vector<position> list_artefact =
    {
        { 0, TAILLE_TERRAIN / 2 },
        { TAILLE_TERRAIN / 2, 0 },
    };

    int i = 0;

    for (auto& p : players_->players)
        if (p->type == rules::PLAYER)
        {
            players_ids_[p->id] = p;
            magic_[p->id] = 0;
            bases_players[p->id] = list_base[i];
            i++;
        }
}

GameState::GameState(const GameState& st)
    : rules::GameState(st)
    , map_(new Map(*st.map_))
    , current_turn_(st.current_turn_)
{
     players_ids_.insert(st.players_ids_.begin(), st.players_ids_.end());
     magic_.insert(st.magic_.begin(), st.magic_.end());
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
    magic_[player] = magic;
}

Map* GameState::get_map() const
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
    for (std::map<int, rules::Player_sptr>::iterator it = players_ids_.begin();
         it != players_ids_.end(); it++)
    {
        owner_base = map_->get_base(it->first)->get_player();
        if (owner_base != it->first)
        {
            // update score of the winner
            if (losers_.find(it->first) != losers_.end())
            {
                players_->players[owner_base]->score += POINTS_VAINQUEUR;
                losers_.insert(it->first);
            }
        }
    }
}

