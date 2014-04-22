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

    EXPECT_EQ(1, gamestate_->get_losers_ids().size())
        << "There should be one losers";

    EXPECT_EQ(0, *(gamestate_->get_losers_ids()).begin())
        << "The loser is the player 0";
}
