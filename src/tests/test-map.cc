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

    INFO("get artefact");

    // Artefact
    EXPECT_EQ(CASE_ARTEFACT, map.get_cell(position { TAILLE_TERRAIN / 2 - 1,
                                          TAILLE_TERRAIN / 2 - 1 })->get_type())
       << "Cell(TAILLE_TERRAIN / 2 - 1, TAILLE_TERRAIN / 2 - 1) should be an artefact";

    // Fontains
    EXPECT_EQ(CASE_FONTAINE, map.get_cell(position { 0, TAILLE_TERRAIN / 2 - 1 })->
              get_type())
        << "Cell(0, TAILLE_TERRAIN / 2 - 1) should be a fontain";

    EXPECT_EQ(CASE_FONTAINE, map.get_cell(position { TAILLE_TERRAIN / 2 - 1, 0  })->
              get_type())
        << "Cell(TAILLE_TERRAIN / 2 - 1, 0) should be a fontain";

    EXPECT_EQ(CASE_FONTAINE, map.get_cell(position { TAILLE_TERRAIN - 1, TAILLE_TERRAIN / 2 - 1  })->
              get_type())
        << "Cell(TAILLE_TERRAIN - 1, TAILLE_TERRAIN / 2 - 1) should be a fontain";

    EXPECT_EQ(CASE_FONTAINE, map.get_cell(position { TAILLE_TERRAIN / 2 - 1, TAILLE_TERRAIN - 1  })->
              get_type())
        << "Cell(TAILLE_TERRAIN - 1 / 2, TAILLE_TERRAIN / 2) should be a fontain";

    // Bases
    EXPECT_EQ(CASE_CASE, map.get_cell(position { 0, 0 })->
              get_type())
        << "Cell(0, 0) should be a base";

    EXPECT_EQ(CASE_CASE, map.get_cell(position { TAILLE_TERRAIN - 1, 0  })->
              get_type())
        << "Cell(TAILLE_TERRAIN -1, 0) should be a base";

    EXPECT_EQ(CASE_CASE, map.get_cell(position { TAILLE_TERRAIN - 1, TAILLE_TERRAIN - 1  })->
              get_type())
        << "Cell(TAILLE_TERRAIN - 1, TAILLE_TERRAIN - 1) should be a base";

    EXPECT_EQ(CASE_CASE, map.get_cell(position { 0, TAILLE_TERRAIN - 1  })->
              get_type())
        << "Cell(0, TAILLE_TERRAIN - 1) should be a base";
}
