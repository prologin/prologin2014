#include <gtest/gtest.h>

#include <utils/log.hh>
#include "../constant.hh"
#include "../map.hh"
#include "../cell.hh"
#include "../game-state.hh"

#include "../action-create.hh"
#include "../action-delete.hh"

class ActionsTest : public ::testing::Test
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
            gamestate_ = new GameState(map_, players);
        }
        Map* map_;
        GameState* gamestate_;
};

//TEST_F(ActionsTest, AttackTest)
//{
//}
//
//TEST_F(ActionsTest, ConstructTest)
//{
//}

TEST_F(ActionsTest, CreateTest)
{
    ActionCreate a1(10, 0);
    EXPECT_EQ(MAGIE_INSUFFISANTE, a1.check(gamestate_))
        << "There shoudn't be enough magic";
    Cell* base_p0 = gamestate_->get_map()->get_base(0);
    EXPECT_EQ(10, base_p0->get_nb_wizards(0))
              << "There should be 10 wizards";
}

TEST_F(ActionsTest, DeleteTest)
{
    ActionDelete a1( { 2, 2 }, 0);
    EXPECT_EQ(OK, a1.check(gamestate_))
        << "It should be possible to put a tower here";

};

//TEST_F(ActionsTest, MoveTest)
//{
//}
//
//TEST_F(ActionsTest, ShootTest)
//{
//}
