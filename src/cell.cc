#include <map>
#include <iostream>

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

void Cell::put_tower(tourelle tower)
{
    tower_ = tower;
    type_ = CASE_TOURELLE;
    player_ = tower.joueur;
}

void Cell::delete_tower(void)
{
    tower_ = { { -1, -1 }, 0, -1, 0, 0 };
    type_ = CASE_SIMPLE;
}
void Cell::set_magic_tower(int attaque)
{
    tower_.attaque = attaque;
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

void Cell::set_wizards(int player, int nb_wizards)
{
    nb_wizards_[player] = nb_wizards;
}

void Cell::set_wizards_movable(int player, int nb_wizards)
{
    nb_wizards_movable_[player] = nb_wizards;
}

int Cell::wizards_attacked(int points, int player)
{
    int wizards_dead = 0;

    for (std::map<int, int>::iterator it = nb_wizards_.begin(); it != nb_wizards_.end(); it++)
    {
        if (it->first != player)
        {
            if (it->second < points)
            {
                wizards_dead += it->second;
                it->second = 0;
            }
            else
            {
                wizards_dead += points;
                it->second -= points;
            }

        }
    }

    return wizards_dead;
}

int Cell::tower_attacked(int points)
{
    if (tower_.vie <= points)
    {
        tower_.vie = 0;
        type_ = CASE_SIMPLE;
        return 1;
    }

    tower_.vie -= points;
    return 0;
}

void Cell::resolve_fight()
{
    int currentMax = 0;
    int currentSecondMax = 0;
    int temp = 0;
    // reser owner of the cell
    player_ = -1;

    // find second
    for (std::map<int, int>::iterator it = nb_wizards_.begin(); it != nb_wizards_.end(); it++)
    {
        if (it->second > currentMax)
            currentMax = it->second;
    }

    for (std::map<int, int>::iterator it = nb_wizards_.begin(); it != nb_wizards_.end(); it++)
    {
        if (it->second > currentSecondMax && it->second < currentMax)
            currentSecondMax = it->second;
    }

    for (std::map<int, int>::iterator it = nb_wizards_.begin(); it != nb_wizards_.end(); it++)
    {
        temp = it->second - currentSecondMax;
        if (temp < 0)
            it->second = 0;
        else
        {
            it->second = temp;
            player_ = it->first;
        }
    }
}

void Cell::resolve_wizard_movable()
{
    nb_wizards_movable_.clear();
}
