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

#ifndef RULES_RULES_HH_
#define RULES_RULES_HH_

#include <rules/actions.hh>
#include <rules/client-messenger.hh>
#include <rules/options.hh>
#include <rules/player.hh>
#include <rules/rules.hh>
#include <rules/server-messenger.hh>
#include <utils/dll.hh>
#include <utils/sandbox.hh>

#include "action-ack.hh"
#include "api.hh"
#include "game-state.hh"

typedef void (*f_champion_partie_debut)();
typedef void (*f_champion_phase_construction)();
typedef void (*f_champion_phase_deplacement)();
typedef void (*f_champion_phase_tirs)();
typedef void (*f_champion_phase_siege)();
typedef void (*f_champion_partie_fin)();

class Rules : public rules::SynchronousRules
{
public:
    explicit Rules(const rules::Options opt);

    // Get the actions structure with registered actions
    rules::Actions* get_actions();

    // Apply an action to the game state
    void apply_action(const rules::IAction&);

    // Check whether the game is over
    bool is_finished();

    void resolve_score();
    void resolve_losers();
    void resolve_fights();
    void resolve_magic();
    void resolve_wizard_movable();
    void resolve_tower_magic();
    void resolve_tower_fighters();
    void resolve_base_released();

    void at_player_start(rules::ClientMessenger_sptr);
    void at_spectator_start(rules::ClientMessenger_sptr);
    void at_player_end(rules::ClientMessenger_sptr);
    void at_spectator_end(rules::ClientMessenger_sptr);
    void player_turn();
    void spectator_turn();

    void end_of_round();
    void start_of_round();
    void at_end();

    void dump_state(std::ostream& out);

protected:
    f_champion_partie_debut champion_partie_debut;
    f_champion_phase_construction champion_phase_construction;
    f_champion_phase_deplacement champion_phase_deplacement;
    f_champion_phase_tirs champion_phase_tirs;
    f_champion_phase_siege champion_phase_siege;
    f_champion_partie_fin champion_partie_fin;

private:
    std::unique_ptr<utils::DLL> champion_dll_;
    std::unique_ptr<Api> api_;
    utils::Sandbox sandbox_;
    rules::Players players_;
};

#endif // !RULES_RULES_HH_
