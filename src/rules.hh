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


class Rules : public rules::TurnBasedRules
{
public:
    explicit Rules(const rules::Options opt);
    virtual ~Rules();

    virtual rules::Actions* get_actions() { return NULL; }
    virtual void apply_action(const rules::IAction_sptr&) {}
    virtual bool is_finished() { return true; }

private:
    utils::DLL* champion_dll_;

    // FIXME
};

#endif // !RULES_RULES_HH_
