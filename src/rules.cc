#include "rules.hh"

Rules::Rules(const rules::Options opt)
    : SynchronousRules(opt),
      champion_dll_(nullptr),
      sandbox_(opt.time)
{
    if (!opt.champion_lib.empty())
    {
        champion_dll_ = new utils::DLL(opt.champion_lib);

        //champion_init = champion_dll_->get<f_champion_init>("init_game");
        //champion_play = champion_dll_->get<f_champion_play>("play_turn");
        //champion_end = champion_dll_->get<f_champion_end>("end_game");

        //sandbox_.execute(champion_init);
    }
    else
        champion_dll_ = nullptr;

    players_ = opt.players;

    // Init map
    Map* map = new Map();

    // Init gamestate
    GameState* game_state = new GameState(map, opt.players);

    // Init api
    api_ = new Api(game_state, opt.player);

    // Register actions
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
    return false;
}

void Rules::resolve_score()
{
    DEBUG("resolve_score");
    std::map<int, rules::Player_sptr> map_players =
        api_->game_state()->get_players_ids();

    for (std::map<int, rules::Player_sptr>::iterator it = map_players.begin();
         it != map_players.end(); it++)
    {
        players_->players[it->first]->score +=
            api_->game_state()->get_nb_fontains(it->first);
    }

    players_->players[api_->game_state()->get_player_artefact()]->score += 4;
    // loop
    //players_->players[1]->score = 1;

    // game->get_player_artefact();
    // get_nb_fontains
}

void Rules::resolve_fights()
{
    api_->game_state()->resolve_fights();
}

//void Rules::end_of_player_turn(uint32_t player_id)
//{
//}

void Rules::end_of_turn()
{
    // construction des tours
    //api_->game_state()->resolve_fights();
    api_->game_state()->resolve_fights();
    resolve_score();
    api_->game_state()->increment_turn();
    //api_->game_state()->clear_wizard_moving();
    // 
}

//void Rules::at_client_start()
//{
//        sandbox_.execute(champion_partie_init);
//d
//}
