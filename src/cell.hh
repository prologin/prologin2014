#ifndef CELL_HH_
#define CELL_HH_

#include <set>
#include <map>

#include "constant.hh"

class Cell
{
    public:
        Cell(int y, int x);

        case_info get_type() const;

        // wizards
        int get_nb_wizards(int player);
        int get_nb_wizards_movable(int player);
        int get_nb_wizards_total();
        void set_wizards(int player, int nb_wizards);
        void set_wizards_movable(int player, int nb_wizards);
        void wizards_attacked(int points, int player);

        void put_tower(tourelle tower);
        void delete_tower(void);
        tourelle get_tower();
        void set_magic_tower(int magic);
        // return true if the tower is destroyed
        bool tower_attacked(int points);

        int get_player() const;

        position get_position() { return { x_, y_ }; }

    private:
        int x_;
        int y_;
        case_info type_;
        int player_;
        // several wizards can be in a cell
        std::map<int, int> nb_wizards_;
        std::map<int, int> nb_wizards_movable_;
        tourelle tower_;
};

#endif // !CELL_H_
