#include <gtest/gtest.h>

#include <utils/log.hh>
#include "../constant.hh"
#include "../map.hh"
#include "../cell.hh"
#include "../game-state.hh"

#include "../action-create.hh"
#include "../action-delete.hh"
#include "../action-construct.hh"
#include "../action-move.hh"
#include "../action-shoot.hh"
#include "../action-attack.hh"

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
            for (auto& p : players->players)
                p->type = rules::PLAYER;

            gamestate_ = new GameState(map_, players);
        }
        Map* map_;
        GameState* gamestate_;
};

// TODO: Check magic gained ?

TEST_F(ActionsTest, AttackTest)
{
    ActionAttack a1({ 2, 2 }, { 2, 5 }, 1);

    EXPECT_EQ(PHASE_INCORRECTE, a1.check(gamestate_))
        << "Wrong phase of the game.";

    gamestate_->setPhase(PHASE_SIEGE);

    EXPECT_EQ(VALEUR_INVALIDE, a1.check(gamestate_))
        << "The distance between the two cells is too far away.";

    ActionAttack a2({ 2, 2 }, { 2, 3 }, 1);
    Cell* c1 = gamestate_->get_map()->get_cell({ 2, 2 });
    Cell* c2 = gamestate_->get_map()->get_cell({ 2, 3 });

    c1->put_tower({ { 2, 2 }, 3, 1, 2, 2 });

    EXPECT_EQ(CASE_UTILISEE, a2.check(gamestate_))
        << "There is a tower on the cell supposed to attack.";

    c1->delete_tower();

    EXPECT_EQ(CASE_VIDE, a2.check(gamestate_))
        << "There is no towers on the cell attacked.";

    c2->put_tower({ { 2, 3 }, 3, 1, 2, 2 });

    EXPECT_EQ(SORCIERS_INSUFFISANTS, a2.check(gamestate_))
        << "There is no wizards on the cell attacking.";

    c1->set_wizards(2, 1);

    EXPECT_EQ(SORCIERS_INSUFFISANTS, a2.check(gamestate_))
        << "Still no wizards on the cell attacking.";

    c1->set_wizards(1, 1);

    EXPECT_EQ(OK, a2.check(gamestate_))
        << "It should be possible to attack a tower.";

    a2.apply_on(gamestate_);

    EXPECT_EQ(CASE_TOURELLE, c2->get_type())
        << "The tower should not be down yet.";

    a2.apply_on(gamestate_);

    EXPECT_EQ(CASE_SIMPLE, gamestate_->get_map()->get_cell({2, 3})->get_type())
        << "The tower should be down now.";

}

TEST_F(ActionsTest, ConstructTest)
{
    gamestate_->set_magic(1, 1000);

    ActionConstruct a1({ 79,
                         1 }, 4, 1);
    EXPECT_EQ(OK, a1.check(gamestate_))
        << "It should be possible to put a tower here";
    a1.apply_on(gamestate_);

    Cell* cbase = gamestate_->get_map()->get_cell({ 79, 1 });

    EXPECT_EQ(CASE_TOURELLE, cbase->get_type())
        << "There should be a tower here.";

    EXPECT_EQ(1, gamestate_->get_towers(1).size())
        << "There should be one tower in the list of towers of the player";

    a1.apply_on(gamestate_);

    EXPECT_EQ(CASE_UTILISEE, a1.check(gamestate_))
        << "The cell is already used.";

    Cell* c2 = gamestate_->get_map()->get_cell({ 2, 3 });
    c2->set_wizards(1, 2);

    ActionConstruct a2({ 2, 3 }, 3, 2);
    EXPECT_EQ(CASE_UTILISEE, a2.check(gamestate_))
        << "There are wizards on the cell";


    Cell* c = gamestate_->get_map()->get_cell({2, 2});
    c->put_tower({ { 2, 2 }, 2, 1, 2, 2 });

    ActionConstruct a4({ 2, 4 }, 4, 1);

    EXPECT_EQ(OK, a4.check(gamestate_))
        << "It should be possible to put a tower here, OK ??";
}

// spawn wizards
TEST_F(ActionsTest, CreateTest)
{
    ActionCreate a1(10, 1);
    EXPECT_EQ(MAGIE_INSUFFISANTE, a1.check(gamestate_))
        << "There shoudn't be enough magic";
    a1.apply_on(gamestate_);
    Cell* base_p0 = gamestate_->get_map()->get_cell(gamestate_->get_base(1));
    EXPECT_EQ(10, base_p0->get_nb_wizards(1))
              << "There should be 10 wizards";
}

// Delete a tower
TEST_F(ActionsTest, DeleteTest)
{
    ActionDelete a1( { 2, 2 }, 1);
    EXPECT_EQ(CASE_VIDE, a1.check(gamestate_))
        << "There are no tower here !";

    Cell* c1 = gamestate_->get_map()->get_cell({ 2, 2 });

    c1->put_tower({ { 2, 2 }, 3, 1, 2, 2 });

    EXPECT_EQ(OK, a1.check(gamestate_))
        << "It should be possible to delete this tower.";


};

// Move wizards
TEST_F(ActionsTest, MoveTest)
{
    ActionMove a1({ 2, 2 }, { 2, 3 }, 3, 1);

    EXPECT_EQ(PHASE_INCORRECTE, a1.check(gamestate_))
        << "Wrong phase of the game.";

    gamestate_->setPhase(PHASE_MOVE);

    EXPECT_EQ(SORCIERS_INSUFFISANTS, a1.check(gamestate_))
        << "There are no wizards on the cell";

    Cell *c1 = gamestate_->get_map()->get_cell({ 2, 2 });
    Cell *c2 = gamestate_->get_map()->get_cell({ 2, 3 });
    c1->set_wizards(1, 10);
    c1->set_wizards_movable(1, 10);

    ActionMove a2({ 2, 2 }, { 2, 3 }, -3, 1);
    EXPECT_EQ(VALEUR_INVALIDE, a2.check(gamestate_))
        << "Negative value";

    a1.apply_on(gamestate_);

    EXPECT_EQ(7, c1->get_nb_wizards(1))
        << "There should be 7 wizards on this cell.";

    EXPECT_EQ(7, c1->get_nb_wizards_movable(1))
        << "There should be 7 wizards movable on this cell.";

    EXPECT_EQ(3, c2->get_nb_wizards(1))
        << "There should be 3 wizards on this cell.";

    EXPECT_EQ(0, c2->get_nb_wizards_movable(1))
        << "There should be no movable on this cell.";

    ActionMove a3({ 2, 5 }, { 2, 4 }, 2, 1);

    Cell *c3 = gamestate_->get_map()->get_cell({ 2, 5 });
    Cell *c4 = gamestate_->get_map()->get_cell({ 2, 4 });

    c3->put_tower({ { 2, 5 }, 3, 1, 2, 2 });

    EXPECT_EQ(CASE_UTILISEE, a3.check(gamestate_))
        << "There is a tower in the initial cell.";

    c3->delete_tower();
    c4->put_tower({ { 2, 4 }, 3, 1, 2, 2 });

    EXPECT_EQ(CASE_UTILISEE, a3.check(gamestate_))
        << "There is a tower in the destination cell.";

    ActionMove a4({ 2, 5 }, { 2, 10 }, 4, 1);

    c3->set_wizards(1, 10);
    c3->set_wizards_movable(1, 10);
    EXPECT_EQ(CASE_IMPOSSIBLE, a4.check(gamestate_))
        << "The cell is too far from the wizards";
}

TEST_F(ActionsTest, ShootTest)
{
    ActionShoot a1(4, { 2, 2 }, { 2, 5 }, 1);

    EXPECT_EQ(PHASE_INCORRECTE, a1.check(gamestate_))
        << "Wrong phase of the game.";

    gamestate_->setPhase(PHASE_SHOOT);

    EXPECT_EQ(CASE_VIDE, a1.check(gamestate_))
        << "There is no tower in this cell.";

    Cell *c1 = gamestate_->get_map()->get_cell({ 2, 2 });
    Cell *c2 = gamestate_->get_map()->get_cell({ 2, 5 });

    c1->put_tower({ { 2, 2 }, 3, 2, 2, 2 });

    EXPECT_EQ(CASE_ADVERSE, a1.check(gamestate_))
        << "This is a tower of another team.";

    c1->put_tower({ { 2, 2 }, 3, 1, 2, 2 });

    EXPECT_EQ(MAGIE_INSUFFISANTE, a1.check(gamestate_))
        << "This is a tower of another team.";

    c1->put_tower({ { 2, 2 }, 3, 1, 2, 5 });

    // Set wizards
    c2->set_wizards(1, 3);
    c2->set_wizards(2, 7);
    c2->set_wizards(3, 3);

    a1.apply_on(gamestate_);

    EXPECT_EQ(3, gamestate_->get_map()->get_cell( { 2, 5 } )->get_nb_wizards(1))
        << "The tower doesn't attack the wizards of its own team.";

    EXPECT_EQ(0, gamestate_->get_map()->get_cell( { 2, 5 } )->get_nb_wizards(0))
        << "There should be no wizards left on this cell.";

    EXPECT_EQ(0, gamestate_->get_map()->get_cell( { 2, 5 } )->get_nb_wizards(3))
        << "There should be 3 wizards left on this cell.";

    EXPECT_EQ(3, gamestate_->get_map()->get_cell( { 2, 5 } )->get_nb_wizards(2))
        << "There should be 3 wizards left on this cell.";

    EXPECT_EQ(MAGIE_INSUFFISANTE, a1.check(gamestate_))
        << "Not enougth magic for the action";

    ActionShoot a2(4, { 2, 2 }, { 2, 7 }, 1);

    EXPECT_EQ(VALEUR_INVALIDE, a2.check(gamestate_))
        << "The cell attacked is too far away.";

}
