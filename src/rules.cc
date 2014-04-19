#include "rules.hh"

Rules::Rules(const rules::Options opt)
    : SynchronousRules(opt),
      champion_dll_(nullptr),
      sandbox_(opt.time)
{
    if (!opt.champion_lib.empty())
    {
        champion_dll_ = new utils::DLL(opt.champion_lib);

        champion_partie_debut = champion_dll_->get<f_champion_partie_debut>("partie_debut");
        champion_jouer_construction = champion_dll_->get<f_champion_jouer_construction>("jouer_construction");
        champion_jouer_deplacement = champion_dll_->get<f_champion_jouer_deplacement>("jouer_deplacement");
        champion_jouer_tirs = champion_dll_->get<f_champion_jouer_tirs>("jouer_tirs");
        champion_jouer_siege = champion_dll_->get<f_champion_jouer_siege>("jouer_siege");
        champion_partie_fin = champion_dll_->get<f_champion_partie_fin>("partie_fin");

        if (opt.player->type == rules::PLAYER)
            sandbox_.execute(champion_partie_debut);
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

    // FIXME: spectator
}

Rules::~Rules()
{
    if (champion_dll_)
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
    int nb_turn = api_->game_state()->get_current_turn();

    return (nb_turn > MAX_TOUR ||
            api_->game_state()->get_losers_ids().size() == 3);
}

void Rules::resolve_score()
{
    DEBUG("resolve_score");

    // losers
    std::unordered_set<int> losers = api_->game_state()->get_losers_ids();

    std::map<int, rules::Player_sptr> map_players =
        api_->game_state()->get_players_ids();
    if (api_->game_state()->get_player_artefact() != -1)
        players_->players[api_->game_state()->get_player_artefact()]->score +=
            POINTS_CONTROLE_ARTEFACT;
    for (std::map<int, rules::Player_sptr>::iterator it = map_players.begin();
         it != map_players.end(); it++)
        if (losers.find(it->first) != losers.end())
            players_->players[it->first]->score += POINTS_SURVIVRE;
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

void Rules::resolve_losers()
{
    DEBUG("resolve_losers");

    api_->game_state()->check_losers();
}

void Rules::resolve_wizard_movable()
{
    DEBUG("resolve_wizard_movable");

    api_->game_state()->resolve_wizard_movable();
}

void Rules::resolve_tower_magic()
{
    DEBUG("resolve_wizard_magic");

    api_->game_state()->resolve_tower_magic();
}

void Rules::end_of_turn()
{
    game_phase phase = api_->game_state()->getPhase();

    switch(phase)
    {
        case PHASE_CONSTRUCTION:
            sandbox_.execute(champion_jouer_construction);
            api_->game_state()->setPhase(PHASE_MOVE);
            break;
        case PHASE_MOVE:
            sandbox_.execute(champion_jouer_deplacement);
            resolve_fights();
            resolve_losers();
            api_->game_state()->setPhase(PHASE_SHOOT);
            break;
        case PHASE_SHOOT:
            sandbox_.execute(champion_jouer_tirs);
            api_->game_state()->setPhase(PHASE_SIEGE);
            break;
        case PHASE_SIEGE:
            sandbox_.execute(champion_jouer_siege);
            resolve_magic();
            resolve_wizard_movable();
            resolve_tower_magic();
            api_->game_state()->increment_turn();
            api_->game_state()->setPhase(PHASE_CONSTRUCTION);
            break;
    }
}

void Rules::at_end()
{
    // Resolve all scores
    resolve_score();
}
