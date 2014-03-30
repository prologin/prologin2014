#include "action-construct.hh"
#include "constant.hh"
#include "game.hh"
#include "tools.hh"

ActionConstruct::ActionConstruct(position pos, int range, int player)
    : pos_(pos)
    , range_(range)
    , player_id_(player)
{
}

ActionConstruct::ActionConstruct()
    : pos_({-1, -1})
    , range_(-1)
    , player_id_(-1)
{
}

int ActionConstruct::check(const GameState* st) const
{
    Cell* cell;
    if (!(cell = st->get_map()->get_cell(pos_)))
        return CASE_IMPOSSIBLE;

    if (cell->get_type() != CASE_SIMPLE || cell->get_nb_wizards_total())
        return CASE_UTILISEE;

    // TODO: more checks

    return OK;
}

void ActionConstruct::handle_buffer(utils::Buffer& buf)
{
    buf.handle(pos_);
    buf.handle(range_);
    buf.handle(player_id_);
}

void ActionConstruct::apply_on(GameState* st) const
{
    // TODO
}