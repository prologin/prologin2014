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
    Cell* cell_dest = st->get_map()->get_cell(start_);

    int nb_movable = cell_start->get_nb_wizards_movable(player_id_);

    if (nb_movable < nb_wizards_)
        return SORCIERS_INSUFFISANTS;

    if (start_.x < 0 || start_.x >= TAILLE_TERRAIN
        || start_.y < 0 || start_.y >= TAILLE_TERRAIN)
        return CASE_IMPOSSIBLE;

    if (dest_.x < 0 || dest_.x >= TAILLE_TERRAIN
        || dest_.y < 0 || dest_.y >= TAILLE_TERRAIN)
        return CASE_IMPOSSIBLE;

    if (cell_start->get_type() != CASE_SIMPLE)
        return CASE_UTILISEE;

    if (cell_dest->get_type() != CASE_SIMPLE)
        return CASE_UTILISEE;

    if (nb_wizards_ < 0)
        return VALEUR_INVALIDE;

    // Checks if start pos and dest pos are adjacents
    bool is_adjacent;
    std::array<position, 4> adjacents{{
        { 0, 1 },
        { 1, 0 },
        { 0, -1 },
        { -1, 0 }
    }};

    for (auto a : adjacents)
    {
        // FIXME
        position next;
        next.x = a.x + start_.x;
        next.y = a.y + start_.y;

        if (next.x == dest_.x && next.y == dest_.y)
            is_adjacent = true;
    }

    // FIXME: Which error ?
    if (!is_adjacent)
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
