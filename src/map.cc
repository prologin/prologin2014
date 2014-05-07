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
    constructing_ = map.constructing_;
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

const Cell* Map::get_cell(position p) const
{
    if (!valid_position(p))
        return NULL;

    return map_[p.y][p.x];
}

Cell* Map::get_cell(position p)
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
            if (map_[x][y]->get_player() == player
                && map_[x][y]->get_type() == CASE_TOURELLE)
                towers.push_back(map_[x][y]->get_tower());

    return towers;
}

static const std::array<position, 4> adjacents{{
    { 0, 1 },
    { 1, 0 },
    { 0, -1 },
    { -1, 0 }
}};

// BFS from pos to find whether the closest tower in CONSTRUCTION_TOURELLE
// range is owned by player.
bool Map::buildable(position pos, int player) const
{
    if (!valid_position(pos))
        return false;

    std::queue<position> todo;
    std::bitset<TAILLE_TERRAIN*TAILLE_TERRAIN> done;
    todo.push(pos);

    position dummy = {-1, -1};
    todo.push(dummy);

    unsigned dist = 0; // Current distance from 'pos'
    bool tower_found = false; // We found a tower which belongs to 'player'

    while (todo.size() > 1)
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
        const Cell *cell = get_cell(cp);

        if (cell->get_type() == CASE_BASE && cell->get_player() == player)
            tower_found = true;
        else if (cell->get_type() == CASE_TOURELLE)
        {
            tourelle t = cell->get_tower();
            if (t.joueur != player) // Enemy tower
                return false;
            else if (constructing_.find(t.pos) == constructing_.end())
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
std::vector<position> Map::path(position start, position end) const
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
                const Cell* cell = get_cell(np);
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
    return map_[TAILLE_TERRAIN / 2][TAILLE_TERRAIN / 2]->get_player();
}

int Map::get_nb_fontains(int player_id)
{
    int nb = 0;

    if (map_[(TAILLE_TERRAIN / 2)][0]->get_player() == player_id)
        nb++;
    if (map_[0][(TAILLE_TERRAIN / 2)]->get_player() == player_id)
        nb++;
    if (map_[(TAILLE_TERRAIN - 1)][(TAILLE_TERRAIN / 2)]->get_player() == player_id)
        nb++;
    if (map_[(TAILLE_TERRAIN / 2)][(TAILLE_TERRAIN - 1)]->get_player() == player_id)
        nb++;

    return nb;
}

void Map::add_constructing(position pos)
{
    constructing_.insert(pos);
}

void Map::resolve_fights()
{
    for (int y = 0; y < TAILLE_TERRAIN; ++y)
        for (int x = 0; x < TAILLE_TERRAIN; ++x)
            if (map_[x][y]->get_type() != CASE_TOURELLE
                && map_[x][y]->get_nb_wizards_total() > 0)
                map_[x][y]->resolve_fight();
}

void Map::resolve_wizard_movable()
{
    for (int y = 0; y < TAILLE_TERRAIN; ++y)
        for (int x = 0; x < TAILLE_TERRAIN; ++x)
            map_[x][y]->resolve_wizard_movable();
}

void Map::resolve_tower_magic()
{
    for (int y = 0; y < TAILLE_TERRAIN; ++y)
        for (int x = 0; x < TAILLE_TERRAIN; ++x)
            if (map_[x][y]->get_type() == CASE_TOURELLE)
                map_[x][y]->set_magic_tower(ATTAQUE_TOURELLE);
}

void Map::resolve_constructing()
{
    constructing_.clear();
}

void Map::delete_all(int player)
{
    for (int y = 0; y < TAILLE_TERRAIN; ++y)
        for (int x = 0; x < TAILLE_TERRAIN; ++x)
        {
            if (map_[x][y]->get_type() == CASE_TOURELLE
                && map_[x][y]->get_player() == player)
                map_[x][y]->delete_tower();
            else
                map_[x][y]->set_wizards(player, 0);
            if (map_[x][y]->get_player() == player)
                map_[x][y]->set_player(-1);
        }
}

void Map::resolve_tower_fighters()
{
    for (int y = 0; y < TAILLE_TERRAIN; ++y)
        for (int x = 0; x < TAILLE_TERRAIN; ++x)
        {
            if (map_[x][y]->get_type() != CASE_TOURELLE)
                map_[x][y]->set_tower_fighters(0);
        }
}

