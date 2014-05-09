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

#include <map>
#include <limits.h>
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
    /* The distance between (2, 2) and (3, 3) is 2, so it's too far away to
     * attack.  */
    const position from_ok       = {2, 2};
    const position from_few      = {1, 3};
    const position from_tower    = {2, 4};
    const position to_far        = {3, 3};
    const position to_no_tower   = {2, 1};
    const position to_ok         = {2, 3};

    Cell &c_from       = *gamestate_->get_map()->get_cell(from_ok);
    Cell &c_from_few   = *gamestate_->get_map()->get_cell(from_few);
    Cell &c_from_tower = *gamestate_->get_map()->get_cell(from_tower);
    Cell &c_far        = *gamestate_->get_map()->get_cell(to_far);
    Cell &c_ok         = *gamestate_->get_map()->get_cell(to_ok);

    const int attack_player = 1;
    const int defense_player = 2;
    /* If two wizards attack the tower, it takes them two rounds to destroy the
     * tower.  */
    const int tower_life = 3;

    c_from.set_wizards(attack_player, 2);
    c_from_tower.set_wizards(attack_player, 3);
    c_from_tower.put_tower(
        {from_tower, 3, defense_player, tower_life, 2});
    c_far.put_tower(
        {to_far,     3, defense_player, tower_life, 2});
    c_ok.put_tower(
        {to_ok,      3, defense_player, tower_life, 2});

    ActionAttack attack_ok(from_ok,            to_ok,       2, attack_player);
    ActionAttack attack_few(from_few,          to_ok,       2, attack_player);
    ActionAttack attack_from_tower(from_tower, to_ok,       2, attack_player);
    ActionAttack attack_no_tower(from_ok,      to_no_tower, 2, attack_player);
    ActionAttack attack_far(from_ok,           to_far,      2, attack_player);

    EXPECT_EQ(PHASE_INCORRECTE, attack_ok.check(gamestate_))
        << "Wrong phase of the game.";
    gamestate_->setPhase(PHASE_SIEGE);

    EXPECT_EQ(SORCIERS_INSUFFISANTS, attack_few.check(gamestate_))
        << "There is no wizards on the cell attacking.";
    c_from_few.set_wizards(attack_player, 1);

    EXPECT_EQ(SORCIERS_INSUFFISANTS, attack_few.check(gamestate_))
        << "Still no wizards on the cell attacking.";
    c_from.set_wizards(attack_player, 3);

    EXPECT_EQ(VALEUR_INVALIDE, attack_far.check(gamestate_))
        << "The distance between the two cells is too far away.";

    EXPECT_EQ(OK, attack_ok.check(gamestate_))
        << "It should be possible to attack a tower.";

    /* TODO: is it really a problem?  */
    EXPECT_EQ(CASE_UTILISEE, attack_from_tower.check(gamestate_))
        << "There is a tower on the cell supposed to attack.";

    EXPECT_EQ(CASE_VIDE, attack_no_tower.check(gamestate_))
        << "There is no tower on the cell attacked.";
    attack_ok.apply_on(gamestate_);

    EXPECT_EQ(CASE_TOURELLE, c_ok.get_type())
        << "The tower should not be down yet.";
    attack_ok.apply_on(gamestate_);

    EXPECT_EQ(CASE_SIMPLE, c_ok.get_type())
        << "The tower should be down now.";
}

TEST_F(ActionsTest, ConstructTest)
{
    const position pos = {30, 1};

    gamestate_->set_magic(1, 1000);

    ActionConstruct a1(pos, 4, 1);
    EXPECT_EQ(OK, a1.check(gamestate_))
        << "It should be possible to put a tower here";
    a1.apply_on(gamestate_);

    Cell* cbase = gamestate_->get_map()->get_cell(pos);

    EXPECT_EQ(CASE_TOURELLE, cbase->get_type())
        << "There should be a tower here.";

    //EXPECT_EQ(1, gamestate_->get_towers(1).size())
    //    << "There should be one tower in the list of towers of the player";

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
    c = gamestate_->get_map()->get_cell({7, 7});
    c->put_tower({ { 7, 7 }, 2, 2, 2, 2 });

    ActionConstruct a4({ 2, 4 }, 4, 1);
    EXPECT_EQ(OK, a4.check(gamestate_))
        << "It should be possible to put a tower here, OK ??";

    ActionConstruct a5({ 4, 2 }, 4, 1);
    EXPECT_EQ(OK, a5.check(gamestate_))
        << "It should be possible to put a tower here";

    ActionConstruct a6({ 4, 4 }, 4, 1);
    EXPECT_EQ(OK, a6.check(gamestate_))
        << "It should be possible to put a tower here";

    ActionConstruct a7({ 5, 4 }, 4, 1);
    EXPECT_EQ(CASE_ADVERSE, a7.check(gamestate_))
        << "It shouldn't be possible to put a tower here";

    ActionConstruct a8({ 4, 5 }, 4, 1);
    EXPECT_EQ(CASE_ADVERSE, a8.check(gamestate_))
        << "It shouldn't be possible to put a tower here";

    ActionConstruct a9({ 5, 5 }, 4, 1);
    EXPECT_EQ(CASE_ADVERSE, a9.check(gamestate_))
        << "It shouldn't be possible to put a tower here";


    a4.apply_on(gamestate_);
    ActionConstruct a10({ 2, 9 }, 4, 1);
    EXPECT_EQ(CASE_ADVERSE, a9.check(gamestate_))
        << "It shouldn't be possible to put a tower here (yet!)";

}

// spawn wizards
TEST_F(ActionsTest, CreateTest)
{
    ActionCreate a1(11, 1);
    EXPECT_EQ(MAGIE_INSUFFISANTE, a1.check(gamestate_))
        << "There shoudn't be enough magic";

    //Check overflow with number of wizards to create
    ActionCreate a2(INT_MAX/2+INT_MAX/4, 1); //So that *2 > INT)MAX
    EXPECT_EQ(MAGIE_INSUFFISANTE, a2.check(gamestate_))
        << "There shoudn't be enough magic";

    ActionCreate a3(10, 1);
    a3.apply_on(gamestate_);
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

TEST_F(ActionsTest, FightTest)
{
    const int player = 1;
    const int other  = 2;

    const position pos = {1, 1};
    Cell &c = *gamestate_->get_map()->get_cell(pos);

    std::map<int, int> scores;
    scores[player] = 0;
    scores[other]  = 0;

    c.set_wizards(player, 10);
    c.set_wizards(other, 5);
    c.resolve_fight(scores);

    EXPECT_EQ(5, c.get_nb_wizards(player))
        << "After the fight, the winner should have 5 wizards left";
    EXPECT_EQ(0, c.get_nb_wizards(other))
        << "After the fight, the loser should have no wizard left";

    EXPECT_EQ(5 * MAGIE_COMBAT, scores[player])
        << "After the fight, the loser shouldn't have earned magic points";
    EXPECT_EQ(0, scores[other])
        << "After the fight, the loser shouldn't have earned magic points";
}
