#include <queue>
#include <set>

#include "map.hh"
#include "cell.hh"
#include "constant.hh"
#include "position.hh"

Map::Map()
{
}

Map::Map(const Map& map)
{
    for (int y = 0; y < TAILLE_TERRAIN; ++y)
        for (int x = 0; x < TAILLE_TERRAIN; ++x)
            map_[x][y] = new Cell(*map.map_[x][y]);
}

Map::~Map()
{
    for (int y = 0; y < TAILLE_TERRAIN; ++y)
        for (int x = 0; x < TAILLE_TERRAIN; ++x)
            delete map_[y][x];
}

bool Map::valid_position(position p) const
{
    return 0 <= p.x && p.x < TAILLE_TERRAIN &&
           0 <= p.y && p.y < TAILLE_TERRAIN;
}


Cell* Map::get_cell(position p) const
{
    if (!valid_position(p))
        return NULL;
    return map_[p.y][p.x];
}

std::vector<tourelle> Map::get_towers(int player)
{
    std::vector<tourelle> towers;
    for (int y = 0; y < TAILLE_TERRAIN; ++y)
        for (int x = 0; x < TAILLE_TERRAIN; ++x)
            if (map_[x][y]->get_player() == player)
                towers.push_back(map_[x][y]->get_tower());

    return towers;
}

Cell* Map::get_base(int player) const
{
    position p = bases_.find(player)->second;
    return get_cell(p);
}

void Map::set_bases_players(std::map<int, position> bases)
{
    bases_ = bases;
}

bool Map::buildable(position pos, int player)
{
    if (!valid_position(pos))
        return false;
    std::array<position, 4> adjacent{{
        { 0, 1 },
        { 1, 0 },
        { 0, -1 },
        { -1, 0 }
    }};
    std::queue<position> todo;
    std::set<position> done;
    todo.push(pos);
    bool tower_found = false; // We found a tower which belongs to 'player'
    int last_dist = 0; // The last distance from the origin

    // BFS loop
    while (todo.empty())
    {
        auto cp = todo.front();
        todo.pop();
        int curr_dist = distance(cp, pos);
        if (curr_dist > CONSTRUCTION_TOURELLE) // Not in build range
            return false;

        // If curr_dist > last_dist, we completed all this "layer", so if we
        // found a tower within this layer, there are no enemy towers on the
        // same layer and so this position is buildable
        if (curr_dist > last_dist && tower_found)
            return true;
        last_dist = curr_dist;

        Cell* cell = get_cell(cp);
        if (cell->get_type() == CASE_TOURELLE)
        {
            if (cell->get_tower().joueur != player) // Enemy tower
                return false;
            else
                tower_found = true; // Tower which belongs to 'player'
        }

        for (auto a : adjacent)
        {
            auto np = cp + a;
            if (valid_position(np) && done.find(np) == done.end())
            {
                todo.push(np);
                done.insert(np);
            }
        }
    }
    return false; // Should not happen: TAILLE_TERRAIN > CONSTRUCTION_TOURELLE
}
