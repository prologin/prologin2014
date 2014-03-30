#include <map>

#include "constant.hh"
//#include "tools.hh"
#include "cell.hh"

Cell::Cell(int y, int x)
    : x_(x),
      y_(y),
      type_(type),
      nb_wizards_(0),
      nb_wizards_movable_(0),
      tower_(NULL)
{
}

terrain Cell::get_type() const
{
    return type_;
}

tourelle Cell::get_tower()
{
    return tower_;
}

int Cell::get_player() const
{
    return player_;
}

erreur Cell::put_tower(tourelle tower)
{
    if (type_ != CASE_SIMPLE)
        return CASE_UTILISEE;
    tower_ = tower;
    return 
}

int Cell::get_nb_wizards(int player)
{
    return nb_wizards_.find(player)->second;
}

int Cell::get_nb_wizards_movable(int player)
{
    return nb_wizards_movable_.find(player)->second;
}

int Cell::get_nb_wizards_total()
{
    int total = 0;

    for (nb_wizards_::iterator it = nb_wizards_.begin(); it != nb_wizards_.end(); it++)
        total += it.second;

    return total;
}

//void Cell::add_wizards(int player, int nb)
//{
//    nb_wizards_ 
//}
