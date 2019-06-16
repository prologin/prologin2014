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
#include <limits.h>
#include <map>

#include "../cell.hh"
#include "../constant.hh"
#include "../game-state.hh"
#include "../map.hh"
#include <utils/log.hh>

#include "../action-attack.hh"
#include "../action-construct.hh"
#include "../action-create.hh"
#include "../action-delete.hh"
#include "../action-move.hh"
#include "../action-shoot.hh"

class ActionsTest : public ::testing::Test
{
protected:
    virtual void SetUp()
    {
        utils::Logger::get().level() = utils::Logger::DEBUG_LEVEL;

        map_ = new Map();

        rules::Players_sptr players(
            new rules::Players{std::vector<rules::Player_sptr>{
                rules::Player_sptr(new rules::Player(0, 0)),
                rules::Player_sptr(new rules::Player(1, 0)),
                rules::Player_sptr(new rules::Player(2, 0)),
                rules::Player_sptr(new rules::Player(3, 0)),
            }});
        for (auto& p : players->players)
            p->type = rules::PLAYER;

        st = std::make_unique<GameState>(map_, players);
    }

    Map* map_;
    std::unique_ptr<GameState> st;
};

// TODO: Check magic gained ?

TEST_F(ActionsTest, AttackTest)
{
    /* The distance between (2, 2) and (3, 3) is 2, so it's too far away to
     * attack.  */
    const position from_ok = {2, 2};
    const position from_few = {1, 3};
    const position from_tower = {2, 4};
    const position to_far = {3, 3};
    const position to_no_tower = {2, 1};
    const position to_ok = {2, 3};

    Cell& c_from = *st->get_map()->get_cell(from_ok);
    Cell& c_from_few = *st->get_map()->get_cell(from_few);
    Cell& c_from_tower = *st->get_map()->get_cell(from_tower);
    Cell& c_far = *st->get_map()->get_cell(to_far);
    Cell& c_ok = *st->get_map()->get_cell(to_ok);

    const int attack_player = 1;
    const int defense_player = 2;
    /* If two wizards attack the tower, it takes them two rounds to destroy the
     * tower.  */
    const int tower_life = 3;

    c_from.set_wizards(attack_player, 2);
    c_from_tower.set_wizards(attack_player, 3);
    c_from_tower.put_tower({from_tower, 3, defense_player, tower_life, 2});
    c_far.put_tower({to_far, 3, defense_player, tower_life, 2});
    c_ok.put_tower({to_ok, 3, defense_player, tower_life, 2});

    ActionAttack attack_ok(from_ok, to_ok, 2, attack_player);
    ActionAttack attack_few(from_few, to_ok, 2, attack_player);
    ActionAttack attack_from_tower(from_tower, to_ok, 2, attack_player);
    ActionAttack attack_no_tower(from_ok, to_no_tower, 2, attack_player);
    ActionAttack attack_far(from_ok, to_far, 2, attack_player);

    EXPECT_EQ(PHASE_INCORRECTE, attack_ok.check(*st))
        << "Wrong phase of the game.";
    st->setPhase(PHASE_SIEGE);

    EXPECT_EQ(SORCIERS_INSUFFISANTS, attack_few.check(*st))
        << "There is no wizards on the cell attacking.";
    c_from_few.set_wizards(attack_player, 1);

    EXPECT_EQ(SORCIERS_INSUFFISANTS, attack_few.check(*st))
        << "Still no wizards on the cell attacking.";
    c_from.set_wizards(attack_player, 3);

    EXPECT_EQ(VALEUR_INVALIDE, attack_far.check(*st))
        << "The distance between the two cells is too far away.";

    EXPECT_EQ(OK, attack_ok.check(*st))
        << "It should be possible to attack a tower.";

    EXPECT_EQ(CASE_UTILISEE, attack_from_tower.check(*st))
        << "There is a tower on the cell supposed to attack.";

    EXPECT_EQ(CASE_VIDE, attack_no_tower.check(*st))
        << "There is no tower on the cell attacked.";
    attack_ok.apply(st);

    EXPECT_EQ(CASE_TOURELLE, c_ok.get_type())
        << "The tower should not be down yet.";

    EXPECT_EQ(SORCIERS_INSUFFISANTS, attack_ok.check(*st))
        << "Every wizards have already attacked the tower.";

    c_from_tower.set_wizards(attack_player, 4);

    attack_ok.apply(st);

    EXPECT_EQ(CASE_SIMPLE, c_ok.get_type()) << "The tower should be down now.";
}

TEST_F(ActionsTest, ConstructTest)
{
    const int player = 0;
    const int other = 1;

    const position pos = {1, 1};
    const position pos_busy = {2, 4};
    const position pos_other = {7, 7};

    Cell* cbase = st->get_map()->get_cell(pos);
    Cell* c2 = st->get_map()->get_cell(pos_busy);
    Cell* c = st->get_map()->get_cell(pos_other);

    ActionConstruct a1(pos, 4, player);
    ActionConstruct a2(pos_busy, 3, player);
    ActionConstruct a4({2, 1}, 4, player);
    ActionConstruct a5({4, 2}, 4, player);
    ActionConstruct a6({3, 2}, 4, player);
    ActionConstruct a7({5, 4}, 4, player);
    ActionConstruct a8({4, 5}, 4, player);
    ActionConstruct a9({5, 6}, 4, player);
    ActionConstruct a10({2, 9}, 4, player);

    st->set_magic(player, 1000);
    c->put_tower({pos_other, 2, other, 2, 2});
    c2->set_wizards(player, 2);

    EXPECT_EQ(OK, a1.check(*st)) << "It should be possible to put a tower here";
    a1.apply(st);

    EXPECT_EQ(CASE_TOURELLE, cbase->get_type())
        << "There should be a tower here.";

    EXPECT_EQ(1, static_cast<int>(st->get_towers(1).size()))
        << "There should be one tower in the list of towers of the player";

    a1.apply(st);

    EXPECT_EQ(CASE_UTILISEE, a1.check(*st)) << "The cell is already used.";

    EXPECT_EQ(CASE_UTILISEE, a2.check(*st)) << "There are wizards on the cell";

    EXPECT_EQ(OK, a4.check(*st)) << "It should be possible to put a tower here";

    EXPECT_EQ(CASE_ADVERSE, a5.check(*st))
        << "The tower is too far from the base: build should not be possible";

    EXPECT_EQ(CASE_ADVERSE, a6.check(*st))
        << ("This tower is near a too recently built one,"
            " so it's too far from base");
    st->get_map()->resolve_constructing();

    EXPECT_EQ(OK, a6.check(*st))
        << ("This time, thanks to the recently built tower,"
            " this cell is buildable");

    EXPECT_EQ(CASE_ADVERSE, a7.check(*st))
        << "It shouldn't be possible to put a tower here";

    EXPECT_EQ(CASE_ADVERSE, a8.check(*st))
        << "It shouldn't be possible to put a tower here: too far";

    EXPECT_EQ(CASE_ADVERSE, a9.check(*st))
        << "It shouldn't be possible to put a tower here: near an enemy's";

    a4.apply(st);
    EXPECT_EQ(CASE_ADVERSE, a9.check(*st))
        << "It shouldn't be possible to put a tower here (yet!)";
}

// spawn wizards
TEST_F(ActionsTest, CreateTest)
{
    ActionCreate a1(11, 1);
    EXPECT_EQ(MAGIE_INSUFFISANTE, a1.check(*st))
        << "There shoudn't be enough magic";

    // Check overflow with number of wizards to create
    ActionCreate a2(INT_MAX / 2 + INT_MAX / 4, 1); // So that *2 > INT_MAX
    EXPECT_EQ(MAGIE_INSUFFISANTE, a2.check(*st))
        << "There shoudn't be enough magic";

    ActionCreate a3(10, 1);
    a3.apply(st);
    Cell* base_p0 = st->get_map()->get_cell(st->get_base(1));
    EXPECT_EQ(10, base_p0->get_nb_wizards(1)) << "There should be 10 wizards";
}

// Delete a tower
TEST_F(ActionsTest, DeleteTest)
{
    ActionDelete a1({2, 2}, 1);
    EXPECT_EQ(CASE_VIDE, a1.check(*st)) << "There are no tower here !";

    Cell* c1 = st->get_map()->get_cell({2, 2});

    c1->put_tower({{2, 2}, 3, 1, 2, 2});

    EXPECT_EQ(OK, a1.check(*st))
        << "It should be possible to delete this tower.";
};

// Move wizards
TEST_F(ActionsTest, MoveTest)
{
    ActionMove a1({2, 2}, {2, 3}, 3, 1);
    ActionMove a2({2, 2}, {2, 3}, -3, 1);
    ActionMove a3({2, 5}, {2, 4}, 2, 1);
    ActionMove a4({2, 5}, {2, 10}, 4, 1);

    Cell* c1 = st->get_map()->get_cell({2, 2});
    Cell* c2 = st->get_map()->get_cell({2, 3});
    Cell* c3 = st->get_map()->get_cell({2, 5});
    Cell* c4 = st->get_map()->get_cell({2, 4});

    EXPECT_EQ(PHASE_INCORRECTE, a1.check(*st)) << "Wrong phase of the game.";

    st->setPhase(PHASE_MOVE);

    EXPECT_EQ(SORCIERS_INSUFFISANTS, a1.check(*st))
        << "There are no wizards on the cell";

    c1->set_wizards(1, 10);
    c1->set_wizards_movable(1, 10);

    EXPECT_EQ(VALEUR_INVALIDE, a2.check(*st)) << "Negative value";

    a1.apply(st);

    EXPECT_EQ(7, c1->get_nb_wizards(1))
        << "There should be 7 wizards on this cell.";

    EXPECT_EQ(7, c1->get_nb_wizards_movable(1))
        << "There should be 7 wizards movable on this cell.";

    EXPECT_EQ(3, c2->get_nb_wizards(1))
        << "There should be 3 wizards on this cell.";

    EXPECT_EQ(0, c2->get_nb_wizards_movable(1))
        << "There should be no movable on this cell.";

    c3->put_tower({{2, 5}, 3, 1, 2, 2});

    EXPECT_EQ(CASE_UTILISEE, a3.check(*st))
        << "There is a tower in the initial cell.";

    c3->delete_tower();
    c4->put_tower({{2, 4}, 3, 1, 2, 2});

    EXPECT_EQ(CASE_UTILISEE, a3.check(*st))
        << "There is a tower in the destination cell.";

    c3->set_wizards(1, 10);
    c3->set_wizards_movable(1, 10);

    EXPECT_EQ(PORTEE_INSUFFISANTE, a4.check(*st))
        << "The cell is too far from the wizards";
}

TEST_F(ActionsTest, ShootTest)
{
    ActionShoot a1(4, {2, 2}, {2, 5}, 1);

    EXPECT_EQ(PHASE_INCORRECTE, a1.check(*st)) << "Wrong phase of the game.";

    st->setPhase(PHASE_SHOOT);

    EXPECT_EQ(CASE_VIDE, a1.check(*st)) << "There is no tower in this cell.";

    Cell* c1 = st->get_map()->get_cell({2, 2});
    Cell* c2 = st->get_map()->get_cell({2, 5});

    c1->put_tower({{2, 2}, 3, 2, 2, 2});

    EXPECT_EQ(CASE_ADVERSE, a1.check(*st))
        << "This is a tower of another team.";
    c1->delete_tower();
    c1->put_tower({{2, 2}, 3, 1, 2, 2});

    EXPECT_EQ(ATTAQUE_INSUFFISANTE, a1.check(*st))
        << "Not enough attack points.";
    c1->delete_tower();
    c1->put_tower({{2, 2}, 3, 1, 2, 5});

    // Set wizards
    c2->set_wizards(1, 3);
    c2->set_wizards(2, 7);
    c2->set_wizards(3, 3);

    a1.apply(st);

    EXPECT_EQ(3, st->get_map()->get_cell({2, 5})->get_nb_wizards(1))
        << "The tower doesn't attack the wizards of its own team.";

    EXPECT_EQ(0, st->get_map()->get_cell({2, 5})->get_nb_wizards(0))
        << "There should be no wizards left on this cell.";

    EXPECT_EQ(0, st->get_map()->get_cell({2, 5})->get_nb_wizards(3))
        << "There should be 3 wizards left on this cell.";

    EXPECT_EQ(3, st->get_map()->get_cell({2, 5})->get_nb_wizards(2))
        << "There should be 3 wizards left on this cell.";

    EXPECT_EQ(ATTAQUE_INSUFFISANTE, a1.check(*st))
        << "Not enough attack points for the action";

    ActionShoot a2(4, {2, 2}, {2, 7}, 1);

    EXPECT_EQ(VALEUR_INVALIDE, a2.check(*st))
        << "The cell attacked is too far away.";
}

TEST_F(ActionsTest, FightTest)
{
    const int player = 1;
    const int other = 2;

    const position pos = {1, 1};
    Cell& c = *st->get_map()->get_cell(pos);

    std::map<int, int> scores;
    scores[player] = 0;
    scores[other] = 0;

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
