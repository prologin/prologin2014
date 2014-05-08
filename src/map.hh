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

#ifndef MAP_HH_
# define MAP_HH_

#include <algorithm>
#include <cassert>
#include <bitset>
#include <queue>
#include <utils/log.hh>
# include <array>
# include <map>
#include <unordered_set>
#include <unordered_map>
# include <vector>
# include <stdlib.h>

# include "constant.hh"
# include "cell.hh"
# include "constant.hh"
# include "position.hh"


class Map
{
    public:
        Map();
        Map(const Map& map);
        ~Map();

        bool valid_position(position p) const;
        const Cell* get_cell(position p) const;
        Cell* get_cell(position p);

        std::vector<tourelle> get_towers(int player) const;

        bool buildable(position pos, int player) const;
        std::vector<position> path(position depart, position) const;

        // Function for the score
        int get_player_artefact();
        int get_nb_fontains(int player_id);

        void add_constructing(position pos);

        void resolve_fights(std::map<int, int> &magic);
        void resolve_wizard_movable();
        void resolve_tower_magic();
        void resolve_constructing();
        void resolve_tower_fighters();

        // delete every element of the player
        void delete_all(int player);
        // Check if a base has been released
    private:
        std::array<std::array<Cell*, TAILLE_TERRAIN>, TAILLE_TERRAIN> map_;

        // Still constructing towers
        std::unordered_set<position, HashPosition> constructing_;

};

#endif /* !MAP_HH_ */
