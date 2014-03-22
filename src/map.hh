#ifndef MAP_HH_
# define MAP_HH_

#include <map>
#include <vector>

#include "constant.hh"
#include "cell.hh"

class Map
{
    public:
        Map();
        Map(const Map& map);
        ~Map();

        bool valid_position(position p) const;
        Cell* get_cell(position p) const;

        std::vector<tourelle> get_towers(int player);

    private:
        std::array<std::array<Cell*, TAILLE_TERRAIN>, TAILLE_TERRAIN> map_;
};

#endif /* !MAP_HH_ */
