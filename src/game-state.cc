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
     towers_.insert(towers_.begin(), st.towers_.begin(), st.towers_.end());
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

bool GameState::add_tower(position pos, int player)
{
    tourelle tower = { pos, PORTEE_TOURELLE, player, VIE_TOURELLE, MAGIE_TOUR };
    towers_.push_back(tower);
    return map_->get_cell(pos)->put_tower(tower);
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


GameState::~GameState()
{
}
