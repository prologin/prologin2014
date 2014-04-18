#include "rules.hh"

Rules::Rules(const rules::Options opt)
    : SynchronousRules(opt),
      champion_dll_(nullptr),
      sandbox_(opt.time)
{
    if (!opt.champion_lib.empty())
    {
        champion_dll_ = new utils::DLL(opt.champion_lib);

//        champion_partie_init = champion_dll_->get<f_champion_partie_init>("partie_init");
//        champion_jouer_construction = champion_dll_->get<f_champion_jouer_construction>("jouer_construction");
//        champion_jouer_supression = champion_dll_->get<f_champion_jouer_supression>("jouer_supression");
//        champion_jouer_tirer = champion_dll_->get<f_champion_jouer_tirer>("jouer_tirer");
//        champion_jouer_creer = champion_dll_->get<f_champion_jouer_creer>("jouer_creer");
//        champion_jouer_deplacement = champion_dll_->get<f_champion_jouer_deplacement>("jouer_deplacement");
//        champion_jouer_attaque = champion_dll_->get<f_champion_jouer_attaque>("jouer_attaque");
//        champion_partie_fin = champion_dll_->get<f_champion_partie_fin>("partie_fin");

        // spectator
        //if (opt.player->type == rules::PLAYER)
        //    sandbox_.execute(champion_partie_init);
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
    DEBUG("resolve_fight");
    api_->game_state()->resolve_fights();
}

void Rules::resolve_magic()
{
    DEBUG("resolve_magic");
    std::map<int, rules::Player_sptr> map_players =
        api_->game_state()->get_players_ids();

    for (std::map<int, rules::Player_sptr>::iterator it = map_players.begin();
         it != map_players.end(); it++)
    {
        api_->game_state()->set_magic(it->first,
                                      api_->game_state()->get_magic(it->first)
                                      + MAGIE_TOUR +
                                      (MAGIE_FONTAINES
                                       * api_->game_state()->
                                       get_nb_fontains(it->first)));
    }
}

void Rules::end_of_turn()
{
    game_phase phase = api_->game_state()->getPhase();

    //api_->actions()->clear();
    //
    switch(phase)
    {
        case PHASE_CONSTRUCTION:
            //sandbox_.execute(champion_jouer_construction);
            break;
        case PHASE_SPAWN:
            //sandbox_.execute(champion_jouer_creer);
            break;
        case PHASE_MOVE:
            //sandbox_.execute(champion_jouer_deplacement);
            break;
        case PHASE_SHOOT:
            //sandbox_.execute(champion_jouer_tirer);
            break;
        case PHASE_ATTACK_TOWER:
            //sandbox_.execute(champion_jouer_attaque);
            break;
    }
    api_->game_state()->increment_turn();

    // construction des tours
    //api_->game_state()->resolve_fights();
    // TODO : Magie gagnee a chaque tour !
    api_->game_state()->resolve_fights();
    resolve_score();
    api_->game_state()->increment_turn();
    //api_->game_state()->clear_wizard_moving();
    // 
}
