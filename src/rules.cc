#include "rules.hh"

Rules::Rules(const rules::Options opt)
    : SynchronousRules(opt)
{
    if (!opt.champion_lib.empty())
        champion_dll_ = new utils::DLL(opt.champion_lib);

    // FIXME
}

Rules::~Rules()
{
    // FIXME

    delete champion_dll_;
}

rules::Actions* Rules::get_actions()
{
    // FIXME
    //return api_->actions();
    return NULL;
}

void Rules::apply_action(const rules::IAction_sptr& action)
{
    api_->game_state_set(action->apply(api_->game_state()));
}

bool Rules::is_finished()
{
    return true;
}
