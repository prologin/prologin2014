#include <algorithm>
#include <cmath>

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

int ActionConstruct::cost() const
{
    int c = COUT_TOURELLE;
    if (range_ > PORTEE_TOURELLE)
        c += std::pow(COUT_PORTEE, range_ - PORTEE_TOURELLE);
    return c;
}

int ActionConstruct::check(const GameState* st) const
{
    if (st->getPhase() != PHASE_CONSTRUCTION)
        return PHASE_INCORRECTE;

    const Cell* cell;

    if (!(cell = st->get_map()->get_cell(pos_)))
        return CASE_IMPOSSIBLE;

    if (cell->get_type() != CASE_SIMPLE || cell->get_nb_wizards_total())
        return CASE_UTILISEE;

    if (!st->get_map()->buildable(pos_, player_id_))
        return CASE_ADVERSE;

    if (range_ < PORTEE_TOURELLE)
        return VALEUR_INVALIDE;

    if (st->get_magic(player_id_) < cost())
        return MAGIE_INSUFFISANTE;

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
    st->get_map()->add_constructing(pos_);
    st->set_magic(player_id_, st->get_magic(player_id_) - cost());
}
