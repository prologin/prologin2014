#include <map>

#include "constant.hh"
//#include "tools.hh"
#include "cell.hh"

Cell::Cell(int y, int x, case_info type)
    : x_(x),
      y_(y),
      type_(type),
      tower_(NULL)
{
}

terrain Cell::get_type() const
{
    return type_;
}

std::set<int>& Cell::get_id_wizards()
{
    return wizards_ids_;
}

int Cell::get_player() const
{
    return player_;
}
