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

//typedef void (*f_champion_partie_init)();
//typedef void (*f_champion_jouer_construction)();
//typedef void (*f_champion_jouer_supression)();
//typedef void (*f_champion_jouer_tirer)();
//typedef void (*f_champion_jouer_creer)();
//typedef void (*f_champion_jouer_deplacement)();
//typedef void (*f_champion_jouer_attaque)();
//typedef void (*f_champion_partie_fin)();

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
        void resolve_fights();
        void resolve_magic();

        void end_of_player_turn(uint32_t player_id);
        void end_of_turn();

    protected:
//        f_champion_partie_init champion_partie_init;
//        f_champion_jouer_construction champion_jouer_construction;
//        f_champion_jouer_supression champion_jouer_supression;
//        f_champion_jouer_tirer champion_jouer_tirer;
//        f_champion_jouer_creer champion_jouer_creer;
//        f_champion_jouer_deplacement champion_jouer_deplacement;
//        f_champion_jouer_attaque champion_jouer_attaque;
//        f_champion_partie_fin champion_partie_fin;

    private:
        utils::DLL* champion_dll_;
        utils::Sandbox sandbox_;
        Api* api_;
        rules::Players_sptr players_;
        // FIXME
};

#endif // !RULES_RULES_HH_
