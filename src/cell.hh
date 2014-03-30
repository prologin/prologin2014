#ifndef CELL_HH_
#define CELL_HH_

#include <set>
#include <map>

#include "constant.hh"

class Cell
{
    public:
        Cell(int y, int x, case_info type);

        case_info get_type() const;

        // wizards
        int get_nb_wizards(int player);
        int get_nb_wizards_movable(int player);
        int get_nb_wizards_total();
        //void add_wizards(int player, int nb);

        bool put_tower(tourelle tower);
        int get_player() const;
        tourelle get_tower();

        position get_position() { return { x_, y_ }; }

    private:
        int x_;
        int y_;
        int player_;
        // several wizards can be in a cell
        std::map<int, int> nb_wizards_;
        std::map<int, int> nb_wizards_movable_;
        tourelle tower_;
};

#endif // !CELL_H_
