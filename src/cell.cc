#include <map>

#include "constant.hh"
//#include "tools.hh"
#include "cell.hh"

Cell::Cell(int y, int x)
    : x_(x),
      y_(y),
      type_(CASE_SIMPLE),
      player_(-1)
{
}

case_info Cell::get_type() const
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

bool Cell::put_tower(tourelle tower)
{
    if (type_ != CASE_SIMPLE)
        return false;

    // FIXME: buildable ?
    //else if (buildable(tower.position, player_)
    //    return false;

    tower_ = tower;
    type_ = CASE_TOURELLE;
    player_ = tower.joueur;
    return true;
}

void Cell::delete_tower(void)
{
    tower_ = { { -1, -1 }, 0, -1, 0, 0 };
    type_ = CASE_TOURELLE;
    // FIXME: player id ?
    player_ = -1;
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

    for (std::map<int, int>::iterator it = nb_wizards_.begin(); it != nb_wizards_.end(); it++)
        total += it->second;

    return total;
}

void Cell::add_wizards(int player, int nb_wizards)
{
    nb_wizards_[player] = nb_wizards;
}
