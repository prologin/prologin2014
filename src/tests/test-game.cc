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
#include "../constant.hh"
#include "../map.hh"
#include "../cell.hh"
#include "../game-state.hh"

class GameTest : public ::testing::Test
{
    protected:
    virtual void SetUp()
    {
        utils::Logger::get().level() = utils::Logger::DEBUG_LEVEL;

        map_ = new Map();

        rules::Players_sptr players(
            new rules::Players {
                std::vector<rules::Player_sptr>
                {
                    rules::Player_sptr(new rules::Player(0, 0)),
                    rules::Player_sptr(new rules::Player(1, 0)),
                    rules::Player_sptr(new rules::Player(2, 0)),
                    rules::Player_sptr(new rules::Player(3, 0)),
                }
            }
        );
        for (auto& p : players->players)
             p->type = rules::PLAYER;

        gamestate_ = new GameState(map_, players);
    }
    Map* map_;
    GameState* gamestate_;
};

TEST_F(GameTest, FightTest1)
{
    Cell* cell_fight = map_->get_cell({ 3, 3 });
    cell_fight->set_wizards(0, 5);
    cell_fight->set_wizards(1, 5);
    cell_fight->set_wizards(2, 5);
    cell_fight->set_wizards(3, 5);

    EXPECT_EQ(20, cell_fight->get_nb_wizards_total())
        << "Wrong number of wizards false";

    gamestate_->resolve_fights();

    EXPECT_EQ(0, cell_fight->get_nb_wizards_total())
        << "Wrong number of wizards after the fight";

    EXPECT_EQ(-1, cell_fight->get_player())
        << "No players on this cell";
}

TEST_F(GameTest, FightTest2)
{
    Cell* cell_fight = map_->get_cell({ 3, 3 });
    cell_fight->set_wizards(0, 1);
    cell_fight->set_wizards(1, 2);
    cell_fight->set_wizards(2, 3);
    cell_fight->set_wizards(3, 4);

    EXPECT_EQ(10, cell_fight->get_nb_wizards_total())
        << "Wrong number of wizards false";

    gamestate_->resolve_fights();

    EXPECT_EQ(1, cell_fight->get_nb_wizards_total())
        << "Wrong number of wizards after the fight";

    EXPECT_EQ(3, cell_fight->get_player())
        << "The 3rd player should own this cell";
}

TEST_F(GameTest, BaseTest)
{
    Cell* cell_base = map_->get_cell(gamestate_->get_base(0));
    cell_base->set_wizards(1, 1);
    gamestate_->resolve_fights();

    gamestate_->check_losers();

    EXPECT_EQ(1ul, gamestate_->get_losers_ids().size())
        << "There should be one losers";

    EXPECT_EQ(0, *(gamestate_->get_losers_ids()).begin())
        << "The loser is the player 0";
}
