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

#ifndef CELL_HH_
#define CELL_HH_

#include <map>
#include <set>

#include "constant.hh"

class Cell
{
public:
    Cell(int y, int x);
    Cell(const Cell& source);
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

    /* Return the player the wizards on this cell belong to.  Must be
     * called only after fights resolution: this way, all wizards are
     * supposed to belong to the same player.  */
    int get_wizards_player() const;

    void put_tower(tourelle tower);
    void delete_tower(void);
    tourelle get_tower() const;
    void set_magic_tower(int attaque);
    // return true if the tower is destroyed
    int tower_attacked(int points);

    int get_player() const;
    void set_player(int player);

    position get_position() { return {x_, y_}; }

    int get_tower_fighters() const;
    void set_tower_fighters(int nb);

    void resolve_fight(std::map<int, int>& magic);
    void resolve_wizard_movable();
    void resolve_towers_attacked();

    // Function to check that a base has been taken (and more importantly,
    // by WHO !
    int get_taken() const;
    void set_taken(int t);

private:
    int x_;
    int y_;
    int player_;
    // several wizards can be in a cell
    std::map<int, int> nb_wizards_;
    std::map<int, int> nb_wizards_movable_;
    tourelle tower_;
    int nb_tower_fighters_;
    int taken_;
};

#endif // !CELL_H_
