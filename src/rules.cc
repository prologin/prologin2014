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
        champion_phase_construction = champion_dll_->get<f_champion_phase_construction>("phase_construction");
        champion_phase_deplacement = champion_dll_->get<f_champion_phase_deplacement>("phase_deplacement");
        champion_phase_tirs = champion_dll_->get<f_champion_phase_tirs>("phase_tirs");
        champion_phase_siege = champion_dll_->get<f_champion_phase_siege>("phase_siege");
        champion_partie_fin = champion_dll_->get<f_champion_partie_fin>("partie_fin");
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
            []() -> rules::IAction* { return new ActionConstruct(); });
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
    api_->actions()->register_action(ID_ACTION_ACK,
            []() -> rules::IAction* { return new ActionAck(); });
}

Rules::~Rules()
{
    if (champion_dll_)
        delete champion_dll_;
    delete api_;
}

void Rules::at_client_start()
{
    sandbox_.execute(champion_partie_debut);
}

void Rules::at_spectator_start()
{
    champion_partie_debut();
}

void Rules::at_client_end()
{
    sandbox_.execute(champion_partie_fin);
}

void Rules::at_spectator_end()
{
    champion_partie_fin();
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

    return (nb_turn > MAX_TOUR);
}

void Rules::resolve_score()
{
    DEBUG("resolve_score");

    // losers
    std::unordered_set<int> losers = api_->game_state()->get_losers_ids();

    std::map<int, rules::Player_sptr> map_players =
        api_->game_state()->get_players_ids();
    for (auto& p : players_->players)
    {
        if (p->type == rules::PLAYER)
        {
            if (api_->game_state()->get_player_artefact() == (int)p->id)
                p->score += POINTS_CONTROLE_ARTEFACT;


            if (losers.find(p->id) == losers.end())
                p->score += POINTS_SURVIVRE;
            p->score += api_->game_state()->get_nb_fontains(p->id) * POINTS_CONTROLE_FONTAINE;
        }
    }
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
        if (!api_->game_state()->has_lost(it->first))
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

void Rules::resolve_tower_fighters()
{
    api_->game_state()->resolve_tower_fighters();
}

void Rules::resolve_base_released()
{
    api_->game_state()->resolve_base_released();
}

void Rules::player_turn()
{
    game_phase phase = api_->game_state()->getPhase();

    switch(phase)
    {
        case PHASE_CONSTRUCTION:
            sandbox_.execute(champion_phase_construction);
            break;
        case PHASE_MOVE:
            sandbox_.execute(champion_phase_deplacement);
            break;
        case PHASE_SHOOT:
            sandbox_.execute(champion_phase_tirs);
            break;
        case PHASE_SIEGE:
            sandbox_.execute(champion_phase_siege);
            break;
    }
}

void Rules::spectator_turn()
{
    game_phase phase = api_->game_state()->getPhase();

    switch(phase)
    {
        case PHASE_CONSTRUCTION:
            champion_phase_construction();
            break;
        case PHASE_MOVE:
            champion_phase_deplacement();
            break;
        case PHASE_SHOOT:
            champion_phase_tirs();
            break;
        case PHASE_SIEGE:
            champion_phase_siege();
            break;
    }

    api_->actions()->add(
                    rules::IAction_sptr(new ActionAck(api_->player()->id)));
}

void Rules::end_of_turn()
{
    game_phase phase = api_->game_state()->getPhase();

    switch(phase)
    {
        case PHASE_CONSTRUCTION:
            api_->game_state()->get_map()->resolve_constructing();
            api_->game_state()->setPhase(PHASE_MOVE);
            break;
        case PHASE_MOVE:
            resolve_fights();
            api_->game_state()->setPhase(PHASE_SHOOT);
            break;
        case PHASE_SHOOT:
            api_->game_state()->setPhase(PHASE_SIEGE);
            break;
        case PHASE_SIEGE:
            resolve_wizard_movable();
            resolve_tower_magic();
            resolve_tower_fighters();
            resolve_base_released();
            resolve_magic();
            api_->game_state()->increment_turn();
            api_->game_state()->setPhase(PHASE_CONSTRUCTION);
            resolve_losers();
            break;
    }

    api_->game_state()->clear_old_version();
}

void Rules::at_end()
{
    // Resolve all scores
    resolve_score();
}

void Rules::start_of_turn()
{
    if (api_->game_state()->getPhase() == PHASE_CONSTRUCTION)
        INFO("TURN %d", api_->game_state()->get_current_turn());
}
