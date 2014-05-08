/*
** Copyright (c) 2014 Mélanie Godard <melanie.godard@prologin.org>
** Copyright (c) 2014 Antoine Pietri <antoine.pietri@prologin.org>
** Copyright (c) 2014 Association Prologin <info@prologin.org>
**
** prologin2014 is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** prologin2014 is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with prologin2014.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <cassert>
#include <map>
#include <iostream>

#include "constant.hh"
//#include "tools.hh"
#include "cell.hh"

Cell::Cell(int y, int x)
    : x_(x)
    , y_(y)
    , player_(-1)
    , tower_({ { -1, -1 }, -1, -1, -1, -1 })
    , nb_tower_fighters_(0)
    , taken_(-1)
{
    nb_wizards_ = std::map<int, int>();
    nb_wizards_movable_ = std::map<int, int>();
}
Cell::Cell(const Cell &c)
    : x_(c.x_)
    , y_(c.y_)
    , player_(c.player_)
    , tower_(c.tower_)
    , nb_tower_fighters_(c.nb_tower_fighters_)
    , taken_(c.taken_)
{
    nb_wizards_.insert(c.nb_wizards_.begin(), c.nb_wizards_.end());
    nb_wizards_movable_.insert(c.nb_wizards_movable_.begin(), c.nb_wizards_movable_.end());
}

Cell::~Cell()
{
}


case_info Cell::get_type() const
{
    // if tower
    if (tower_.vie > 0)
        return CASE_TOURELLE;

    // if base
    if ((x_ == 0 && (y_ == 0 || y_ == TAILLE_TERRAIN - 1))
        || (x_ == TAILLE_TERRAIN - 1 && (y_ == TAILLE_TERRAIN - 1 || y_ == 0)))
            return CASE_BASE;

    // if fontain
    if (((x_ == TAILLE_TERRAIN / 2)
         && (y_ == TAILLE_TERRAIN - 1 || y_ == 0))
        || ((x_ == 0 || x_ == TAILLE_TERRAIN - 1) && y_ == TAILLE_TERRAIN / 2))
        return CASE_FONTAINE;

    // if artefact
    if (x_ == TAILLE_TERRAIN / 2 && y_ == TAILLE_TERRAIN / 2)
        return CASE_ARTEFACT;

    return CASE_SIMPLE;
}

tourelle Cell::get_tower() const
{
    return tower_;
}

int Cell::get_player() const
{
    return player_;
}

void Cell::set_player(int player)
{
    player_ = player;
}

void Cell::put_tower(tourelle tower)
{
    tower_ = tower;
    player_ = tower_.joueur;
}

void Cell::delete_tower(void)
{
    tower_ = { { -1, -1 }, -1, -1, -1, -1 };
    player_ = -1;
}
void Cell::set_magic_tower(int attaque)
{
    tower_.attaque = attaque;
}

int Cell::get_nb_wizards(int player) const
{
    int nb = nb_wizards_.find(player)->second;
    if (nb < 0)
        return 0;
    return nb;
}

int Cell::get_nb_wizards_movable(int player) const
{
    int nb = nb_wizards_movable_.find(player)->second;
    if (nb < 0)
        return 0;
    return nb;
}

int Cell::get_nb_wizards_total() const
{
    int total = 0;

    for (const auto& it : nb_wizards_)
        total += it.second;

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

int Cell::get_wizards_player() const
{
    int player = -1;
    for (const auto& it : nb_wizards_)
        if (it.second > 0)
        {
            assert (player == -1 || player == it.first);
            player = it.first;
        }
    for (const auto& it : nb_wizards_movable_)
        if (it.second > 0)
        {
            assert (player == -1 || player == it.first);
            player = it.first;
        }
    return player;
}

int Cell::tower_attacked(int points)
{
    if (tower_.vie <= points)
    {
        tower_ = { { -1, -1 }, -1, -1, -1, -1 };
        player_ = -1;
        return 1;
    }

    tower_.vie -= points;
    return 0;
}

void Cell::resolve_fight()
{
    int currentMax = 0;
    int currentSecondMax = 0;
    int idcurrentMax = -1;

    // find second
    for (std::map<int, int>::iterator it = nb_wizards_.begin(); it != nb_wizards_.end(); it++)
    {
        if (it->second >= currentMax)
        {
            currentSecondMax = currentMax;
            currentMax = it->second;
            idcurrentMax = it->first;
        }
    }

    nb_wizards_.clear();

    if (currentMax - currentSecondMax > 0)
        nb_wizards_[idcurrentMax] = currentMax - currentSecondMax;

    if (get_type() != CASE_BASE)
    {
        if (nb_wizards_.empty())
            player_ = -1;
        else
            player_ = idcurrentMax;
    }
    else if (!nb_wizards_.empty() && idcurrentMax != player_)
    {
        player_ = -1;
        taken_ = idcurrentMax;
    }
}

void Cell::resolve_wizard_movable()
{
    nb_wizards_movable_.clear();
    if (player_ != -1)
        nb_wizards_movable_[player_] = nb_wizards_[player_];
}

void Cell::resolve_towers_attacked()
{
    nb_tower_fighters_ = nb_wizards_.size();
}

int Cell::get_tower_fighters() const
{
    return nb_tower_fighters_;
}
void Cell::set_tower_fighters(int nb)
{
    nb_tower_fighters_ = nb;
}
int Cell::get_taken() const
{
    return taken_;
}
