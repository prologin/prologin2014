#include "action-construct.hh"

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
    if (st->getPhase() != PHASE_CONSTRUCTION)
        return PHASE_INCORRECTE;

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
    tourelle t = 
    {
        {
            pos_.x,
            pos_.y
        },
        range_,
        player_id_,
        VIE_TOURELLE,
        ATTAQUE_TOURELLE
    };
    st->get_map()->get_cell(pos_)->put_tower(t);
}
