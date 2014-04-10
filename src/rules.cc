#include "rules.hh"

Rules::Rules(const rules::Options opt)
    : SynchronousRules(opt)
{
    if (!opt.champion_lib.empty())
        champion_dll_ = new utils::DLL(opt.champion_lib);
    else
        champion_dll_ = nullptr;

    Map* map = new Map();
    GameState* game_state = new GameState(map, opt.players);

    api_ = new Api(game_state, opt.player);

    api_->actions()->register_action(ID_ACTION_CONSTRUCT,
            []() -> rules::IAction* { return new ActionAttack(); });
    api_->actions()->register_action(ID_ACTION_DELETE,
            []() -> rules::IAction* { return new ActionDelete(); });
    api_->actions()->register_action(ID_ACTION_SHOOT,
            []() -> rules::IAction* { return new ActionShoot(); });
    api_->actions()->register_action(ID_ACTION_CREATE,
            []() -> rules::IAction* { return new ActionCreate(); });
    api_->actions()->register_action(ID_ACTION_MOVE,
            []() -> rules::IAction* { return new ActionMove(); });
    api_->actions()->register_action(ID_ACTION_ATTACK,
            []() -> rules::IAction* { return new ActionAttack(); });

    // FIXME
}

Rules::~Rules()
{
    // FIXME

    delete champion_dll_;
}

rules::Actions* Rules::get_actions()
{
    return api_->actions();
}

void Rules::apply_action(const rules::IAction_sptr& action)
{
    api_->game_state_set(action->apply(api_->game_state()));
}

bool Rules::is_finished()
{
    return true;
}
