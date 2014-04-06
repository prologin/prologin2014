#ifndef MAP_HH_
# define MAP_HH_

# include <array>
# include <map>
# include <vector>
# include <stdlib.h>

# include "constant.hh"
# include "cell.hh"

class Map
{
    public:
        Map();
        Map(const Map& map);
        ~Map();

        bool valid_position(position p) const;
        Cell* get_cell(position p) const;

        std::vector<tourelle> get_towers(int player);

        // base of the player
        Cell* get_base(int player) const;
        void set_bases_players(std::map<int, position> bases);
        bool buildable(position pos, int player);
        std::vector<position> path(position depart, position);

    private:
        std::array<std::array<Cell*, TAILLE_TERRAIN>, TAILLE_TERRAIN> map_;
        std::map<int, position> bases_;
};

#endif /* !MAP_HH_ */
