#ifndef CELL_HH_
#define CELL_HH_

#include <set>
#include <map>

#include "constant.hh"

class Cell
{
    public:
        Cell(int y, int x);
        Cell(const Cell &source);
        ~Cell();

        case_info get_type() const;

        // wizards
        int get_nb_wizards(int player) const;
        int get_nb_wizards_movable(int player) const;
        int get_nb_wizards_total() const;
        void set_wizards(int player, int nb_wizards);
        void set_wizards_movable(int player, int nb_wizards);
        // return nb_wizards dead
        int wizards_attacked(int points, int player);

        void put_tower(tourelle tower);
        void delete_tower(void);
        tourelle get_tower() const;
        void set_magic_tower(int attaque);
        // return true if the tower is destroyed
        int tower_attacked(int points);

        int get_player() const;
        void set_player(int player);

        position get_position() { return { x_, y_ }; }

        void resolve_fight();
        void resolve_wizard_movable();
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
