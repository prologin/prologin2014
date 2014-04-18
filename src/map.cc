#include "map.hh"

Map::Map()
{
    for (int y = 0; y < TAILLE_TERRAIN; ++y)
        for (int x = 0; x < TAILLE_TERRAIN; ++x)
            map_[x][y] = new Cell(x, y);
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
            delete map_[x][y];
}

bool Map::valid_position(position p) const
{
    return 0 <= p.x && p.x < TAILLE_TERRAIN &&
           0 <= p.y && p.y < TAILLE_TERRAIN;
}

Cell* Map::get_cell(position p)
{
    if (!valid_position(p))
        return NULL;
    return map_[p.x][p.y];
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

Cell* Map::get_base(int player)
{
    position p = bases_.find(player)->second;
    return get_cell(p);
}

void Map::set_bases_players(std::map<int, position> bases)
{
    bases_ = bases;
}

static const std::array<position, 4> adjacents{{
    { 0, 1 },
    { 1, 0 },
    { 0, -1 },
    { -1, 0 }
}};

// BFS from pos to find whether the closest tower in CONSTRUCTION_TOURELLE
// range is owned by player.
bool Map::buildable(position pos, int player)
{
    if (!valid_position(pos))
        return false;
    Cell *cell = get_cell(pos);
    if (cell->get_type() == CASE_TOURELLE)
        return false;
    // TODO check whether there is a unit

    std::queue<position> todo;
    std::bitset<TAILLE_TERRAIN*TAILLE_TERRAIN> done;
    todo.push(pos);

    position dummy = {-1, -1};
    todo.push(dummy);

    unsigned dist = 0; // Current distance from 'pos'
    bool tower_found = false; // We found a tower which belongs to 'player'

    while (todo.size()>1)
    {
        position cp = todo.front();
        todo.pop();

        if (cp == dummy)
        {
            if (tower_found)
                return true;
            ++dist;
            if (dist > CONSTRUCTION_TOURELLE)
                return false;
            todo.push(dummy);
            continue;
        }

        Cell *cell = get_cell(cp);
        if (cell->get_type() == CASE_TOURELLE)
        {
            if (cell->get_tower().joueur != player) // Enemy tower
                return false;
            else
                tower_found = true; // Tower which belongs to 'player'
        }

        for (auto a : adjacents)
        {
            position np = cp + a;
            unsigned coord = np.y * TAILLE_TERRAIN + np.x;
            if (valid_position(np) && !done[coord])
            {
                todo.push(np);
                done.set(coord);
            }
        }
    }
    assert(false); // Should not happen: TAILLE_TERRAIN > CONSTRUCTION_TOURELLE
    return false;
}

// BFS to find the shortest path between start and end
// Note : An empty path can mean that start and end are not connected or that
// start == end.
// Note : A path will be returned even if a tower is present at one of its
//  extremities.
std::vector<position> Map::path(position start, position end)
{
    if (!valid_position(start) || !valid_position(end) || start == end)
        return {};

    std::queue<position> todo;
    std::bitset<TAILLE_TERRAIN * TAILLE_TERRAIN> done;
    std::bitset<TAILLE_TERRAIN * TAILLE_TERRAIN * 2> parent;
    todo.push(start);

    position dummy = {-1, -1};
    todo.push(dummy);

    unsigned dist = 1;

    while (todo.size() > 1)
    {
        position cp = todo.front();
        todo.pop();

        if (cp == dummy)
        {
            ++dist;
            todo.push(dummy);
            continue;
        }

        for (unsigned char a = 0; a < 4; ++a)
        {
            position np = cp + adjacents[a];
            unsigned coord = np.y * TAILLE_TERRAIN + np.x;
            if (valid_position(np) && !done[coord])
            {
                Cell* cell = get_cell(np);
                if (cell->get_type() != CASE_TOURELLE)
                {
                    parent.set(coord * 2, a & 1);
                    parent.set(coord * 2 + 1, a & 2);
                    todo.push(np);
                    done.set(coord);
                }

                if (np == end)
                {
                    std::vector<position> ret;
                    parent.set(coord * 2, a & 1);
                    parent.set(coord * 2 + 1, a & 2);
                    position bt_curr = end;
                    ret.resize(dist);
                    auto it = ret.rbegin();
                    while (bt_curr != start)
                    {
                        *it++ = bt_curr;
                        unsigned coord = (bt_curr.y * TAILLE_TERRAIN +
                                          bt_curr.x) * 2;
                        bt_curr -= adjacents[parent[coord] +
                                             parent[coord + 1] * 2];
                    }
                    return ret;
                }
            }
        }
    }
    return {}; // No path found
}

int Map::get_player_artefact()
{
    return map_[TAILLE_TERRAIN / 2 - 1][TAILLE_TERRAIN / 2 - 1]->get_player();
}

int Map::get_nb_fontains(int player_id)
{
    int nb = 0;

    if (map_[(TAILLE_TERRAIN / 2) - 1][0]->get_player() == player_id)
        nb++;
    if (map_[0][(TAILLE_TERRAIN / 2) - 1]->get_player() == player_id)
        nb++;
    if (map_[(TAILLE_TERRAIN - 1) - 1][(TAILLE_TERRAIN / 2) - 1]->get_player() == player_id)
        nb++;
    if (map_[(TAILLE_TERRAIN / 2) - 1][(TAILLE_TERRAIN - 1) - 1]->get_player() == player_id)
        nb++;

    return nb;
}

void Map::resolve_fights()
{
    for (int y = 0; y < TAILLE_TERRAIN; ++y)
        for (int x = 0; x < TAILLE_TERRAIN; ++x)
            map_[x][y]->resolve_fight();
}
