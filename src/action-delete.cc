# include "action-delete.hh"

ActionDelete::ActionDelete(position pos, int player)
    : position_(pos)
    , player_id_(player)
{
}

ActionDelete::ActionDelete()
    : position_({ -1, -1 })
    , player_id_(-1)
{
}

int ActionDelete::check(const GameState* st) const
{
    Cell* cell = st->get_map()->get_cell(position_);

    if (position_.x < 0 || position_.x >= TAILLE_TERRAIN
        || position_.y < 0 || position_.y >= TAILLE_TERRAIN)
        return CASE_IMPOSSIBLE;

    else if (cell->get_type() != CASE_TOURELLE)
        return CASE_VIDE;

    else if (cell->get_player() != player_id_)
        return CASE_ADVERSE;

    return OK;
}

void ActionDelete::handle_buffer(utils::Buffer& buf)
{
    buf.handle(position_);
    buf.handle(player_id_);
}

void ActionDelete::apply_on(GameState* gameState) const
{
    Cell* cell = gameState->get_map()->get_cell(position_);
    cell->delete_tower();
    // FIXME: set magic of the player ?
}
