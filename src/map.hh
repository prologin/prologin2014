#ifndef MAP_HH_
# define MAP_HH_

#include <algorithm>
#include <cassert>
#include <bitset>
#include <queue>
#include <utils/log.hh>
# include <array>
# include <map>
#include <unordered_set>
#include <unordered_map>
# include <vector>
# include <stdlib.h>

# include "constant.hh"
# include "cell.hh"
# include "constant.hh"
# include "position.hh"


class Map
{
    public:
        Map();
        Map(const Map& map);
        ~Map();

        bool valid_position(position p) const;
        Cell* get_cell(position p);

        std::vector<tourelle> get_towers(int player);

        // base of the player
        Cell* get_base(int player);
        void set_bases_players(std::map<int, position> bases);
        bool buildable(position pos, int player);
        std::vector<position> path(position depart, position);

        // Function for the score
        int get_player_artefact();
        int get_nb_fontains(int player_id);

        void resolve_fights();
    private:
        std::array<std::array<Cell*, TAILLE_TERRAIN>, TAILLE_TERRAIN> map_;
        std::map<int, position> bases_;
};

#endif /* !MAP_HH_ */
