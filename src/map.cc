#include "map.hh"

Map::Map()
{
}

Map::Map(const Map& map)
    : fontains_(map.fontains_),
      artefacts_(map.artefacts_)
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

std::vector<position> Map::get_fontains() const
{
    return fontains_;
}

std::vector<position> Map::get_artefacts() const
{
    return artefacts_;
}

