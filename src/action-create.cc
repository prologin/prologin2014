# include "action-create.hh"

ActionCreate::ActionCreate(int nb_wizards, int player)
    : nb_wizards_(nb_wizards)
    , player_id_(player)
{
}

ActionCreate::ActionCreate()
    : nb_wizards_(-1)
    , player_id_(-1)
{
}

int ActionCreate::check(const GameState* st) const
{
    if (nb_wizards_ < = 0)
        return VALEUR_INVALIDE;

    if (st->get_magic() < COUT_SORCIER * nb_wizards_)
        return MAGIE_INSUFFISANTE;

    return OK;
}

void ActionCreate::handle_buffer(utils::Buffer& buf)
{
}

void ActionCreate::apply_on(GameState* gameState) const
{
}
