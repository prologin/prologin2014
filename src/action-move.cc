#include "action-move.hh"

ActionMove::ActionMove(position start, position dest, int nb_wizards, int player)
    : start_(start)
    , dest_(dest)
    , nb_wizards_(nb_wizards)
    , player_id_(player)
{
}

ActionMove::ActionMove()
    : start_({ -1, -1 })
    , dest_({ -1, -1 })
    , nb_wizards_(-1)
    , player_id_(-1)
{
}

int ActionMove::check(const GameState* st) const
{
    Cell* cell_start = st->get_map()->get_cell(start_);
    Cell* cell_dest = st->get_map()->get_cell(dest_);

    int nb_movable = cell_start->get_nb_wizards_movable(player_id_);

    if (nb_wizards_ < 0)
        return VALEUR_INVALIDE;

    if (cell_start->get_type() == CASE_TOURELLE)
        return CASE_UTILISEE;

    if (cell_dest->get_type() == CASE_TOURELLE)
        return CASE_UTILISEE;

    if (nb_movable < nb_wizards_)
        return SORCIERS_INSUFFISANTS;

    if (start_.x < 0 || start_.x >= TAILLE_TERRAIN
        || start_.y < 0 || start_.y >= TAILLE_TERRAIN)
        return CASE_IMPOSSIBLE;

    if (dest_.x < 0 || dest_.x >= TAILLE_TERRAIN
        || dest_.y < 0 || dest_.y >= TAILLE_TERRAIN)
        return CASE_IMPOSSIBLE;

    std::vector<position> path = st->get_map()->path(start_, dest_);

    // Checks if the cell is not too far from the wizard
    if (path.size() > PORTEE_SORCIER)
        return CASE_IMPOSSIBLE;

    return OK;
}

void ActionMove::handle_buffer(utils::Buffer& buf)
{
    buf.handle(start_);
    buf.handle(dest_);
    buf.handle(nb_wizards_);
    buf.handle(player_id_);
}

void ActionMove::apply_on(GameState* gameState) const
{
    Cell* cell_start = gameState->get_map()->get_cell(start_);
    Cell* cell_dest = gameState->get_map()->get_cell(dest_);


    cell_start->set_wizards(player_id_,
                            cell_start->get_nb_wizards(player_id_)
                            - nb_wizards_);
    cell_start->set_wizards_movable(player_id_,
                                    cell_start->
                                    get_nb_wizards_movable(player_id_)
                                    - nb_wizards_);
    cell_dest->set_wizards(player_id_,
                            cell_dest->get_nb_wizards(player_id_)
                            + nb_wizards_);
}
