#include "map.hh"

Map::Map()
{
}

Map::Map(const Map& map)
{
}

Map::~Map()
{
}

bool Map::valid_position(position p) const
{
    return 0 <= p.x && p.x < TAILLE_TERRAIN &&
           0 <= p.y && p.y < TAILLE_TERRAIN;
}
