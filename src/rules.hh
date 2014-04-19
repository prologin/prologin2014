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
# include "api.hh"

typedef void (*f_champion_partie_debut)();
typedef void (*f_champion_jouer_construction)();
typedef void (*f_champion_jouer_deplacement)();
typedef void (*f_champion_jouer_tirs)();
typedef void (*f_champion_jouer_siege)();
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

        void end_of_player_turn(uint32_t player_id);
        void end_of_turn();
        void at_end();

    protected:
        f_champion_partie_debut champion_partie_debut;
        f_champion_jouer_construction champion_jouer_construction;
        f_champion_jouer_deplacement champion_jouer_deplacement;
        f_champion_jouer_tirs champion_jouer_tirs;
        f_champion_jouer_siege champion_jouer_siege;
        f_champion_partie_fin champion_partie_fin;

    private:
        utils::DLL* champion_dll_;
        utils::Sandbox sandbox_;
        Api* api_;
        rules::Players_sptr players_;
        // FIXME
};

#endif // !RULES_RULES_HH_
