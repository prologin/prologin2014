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
# define RULES_RULES_HH_

# include <utils/dll.hh>
# include <utils/sandbox.hh>
# include <rules/options.hh>
# include <rules/client-messenger.hh>
# include <rules/server-messenger.hh>
# include <rules/player.hh>
# include <rules/rules.hh>
# include <rules/actions.hh>

# include "game-state.hh"
# include "action-ack.hh"
# include "api.hh"

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
        virtual ~Rules();

        // Get the actions structure with registered actions
        virtual rules::Actions* get_actions();

        // Apply an action to the game state
        virtual void apply_action(const rules::IAction_sptr&);

        // Check whether the game is over
        virtual bool is_finished();

        void resolve_score();
        void resolve_losers();
        void resolve_fights();
        void resolve_magic();
        void resolve_wizard_movable();
        void resolve_tower_magic();
        void resolve_tower_fighters();
        void resolve_base_released();

        void at_client_start();
        void at_spectator_start();
        void at_client_end();
        void at_spectator_end();
        void player_turn();
        void spectator_turn();

        void end_of_turn();
        void start_of_turn();
        void at_end();

    protected:
        f_champion_partie_debut champion_partie_debut;
        f_champion_phase_construction champion_phase_construction;
        f_champion_phase_deplacement champion_phase_deplacement;
        f_champion_phase_tirs champion_phase_tirs;
        f_champion_phase_siege champion_phase_siege;
        f_champion_partie_fin champion_partie_fin;

    private:
        utils::DLL* champion_dll_;
        utils::Sandbox sandbox_;
        Api* api_;
        rules::Players_sptr players_;
        // FIXME
};

#endif // !RULES_RULES_HH_
