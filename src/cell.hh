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

        // FIXME (personalized errors)
        bool put_tower(tourelle tower);
        int get_player() const;
        std::set<int>& get_id_wizards();
        tourelle get_tourelle();

    private:
        int x;
        int y;
        case_info type_;
        int player_;
        // several wizards can be in a cell
        std::set<int> wizards_ids_;
        tourelle tower_;
};

#endif // !CELL_H_
