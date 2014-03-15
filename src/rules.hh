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

class Rules : public rules::TurnBasedRules
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

private:
    utils::DLL* champion_dll_;

    Api* api_;
    // FIXME
};

#endif // !RULES_RULES_HH_
