#include <gtest/gtest.h>

#include "../game-state.hh"
#include <utils/log.hh>

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
            gamestate_ = new GameState(map_, players);
        }
        std::stringstream f;
        Map* map;
        GameState* gamestate_;
}

TEST_F(ActionsTest, AttackTest)
{
}

TEST_F(ActionsTest, ConstructTest)
{
}

TEST_F(ActionsTest, CreateTest)
{
}

TEST_F(ActionsTest, DeleteTest)
{
}

TEST_F(ActionsTest, MoveTest)
{
}

TEST_F(ActionsTest, ShootTest)
{
}
