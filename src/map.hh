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

        std::vector<position> get_fontains() const;
        std::vector<position> get_artefacts() const;
    private:
        std::array<std::array<Cell*, TAILLE_TERRAIN>, TAILLE_TERRAIN> map_;

        std::vector<position> fontains_;
        std::vector<position> artefacts_;
};

// TODO
// load a map ?

#endif /* !MAP_HH_ */
