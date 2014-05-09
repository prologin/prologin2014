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

#include <gtest/gtest.h>
#include <utils/log.hh>
#include "../game-state.hh"
#include "../map.hh"

class MapTest : public ::testing::Test
{
    protected:
        virtual void SetUp()
        {
            utils::Logger::get().level() = utils::Logger::DEBUG_LEVEL;

        }
};

TEST_F(MapTest, MapCreated)
{
    Map map;

    // Artefact
    EXPECT_EQ(CASE_ARTEFACT, map.get_cell(position { TAILLE_TERRAIN / 2,
                                          TAILLE_TERRAIN / 2 })->get_type())
       << "Cell(TAILLE_TERRAIN / 2 - 1, TAILLE_TERRAIN / 2 - 1) should be an artefact";

    // Fontains
    EXPECT_EQ(CASE_FONTAINE, map.get_cell(position { 0, TAILLE_TERRAIN / 2 })->
              get_type())
        << "Cell(0, TAILLE_TERRAIN / 2 - 1) should be a fontain";

    EXPECT_EQ(CASE_FONTAINE, map.get_cell(position { TAILLE_TERRAIN / 2 , 0  })->
              get_type())
        << "Cell(TAILLE_TERRAIN / 2 - 1, 0) should be a fontain";

    EXPECT_EQ(CASE_FONTAINE, map.get_cell(position { TAILLE_TERRAIN - 1, TAILLE_TERRAIN / 2 })->
              get_type())
        << "Cell(TAILLE_TERRAIN - 1, TAILLE_TERRAIN / 2 - 1) should be a fontain";

    EXPECT_EQ(CASE_FONTAINE, map.get_cell(position { TAILLE_TERRAIN / 2, TAILLE_TERRAIN - 1  })->
              get_type())
        << "Cell(TAILLE_TERRAIN - 1 / 2, TAILLE_TERRAIN / 2) should be a fontain";

    // Bases
    EXPECT_EQ(CASE_BASE, map.get_cell(position { 0, 0 })->
              get_type())
        << "Cell(0, 0) should be a base";

    EXPECT_EQ(CASE_BASE, map.get_cell(position { TAILLE_TERRAIN - 1, 0  })->
              get_type())
        << "Cell(TAILLE_TERRAIN -1, 0) should be a base";

    EXPECT_EQ(CASE_BASE, map.get_cell(position { TAILLE_TERRAIN - 1, TAILLE_TERRAIN - 1  })->
              get_type())
        << "Cell(TAILLE_TERRAIN - 1, TAILLE_TERRAIN - 1) should be a base";

    EXPECT_EQ(CASE_BASE, map.get_cell(position { 0, TAILLE_TERRAIN - 1  })->
              get_type())
        << "Cell(0, TAILLE_TERRAIN - 1) should be a base";
}

TEST_F(MapTest, CellStat)
{
    Map map;

    EXPECT_EQ(0, map.get_cell(position { 2, 2})->get_nb_wizards(0))
        << "The number of wizards on this cell shoulb be 0";
}
