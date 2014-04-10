# include "action-shoot.hh"

ActionShoot::ActionShoot(int        points,
                         position   tower,
                         position   target,
                         int        player)
    : points_(points)
    , tower_(tower)
    , target_(target)
    , player_id_(player)
{
}

ActionShoot::ActionShoot()
    : points_(-1)
    , tower_({ -1, -1 })
    , target_({ -1, -1 })
    , player_id_(-1)
{
}

int ActionShoot::check(const GameState* st) const
{
    Cell* cell_tower = st->get_map()->get_cell(tower_);

    if (tower_.x < 0 || tower_.x >= TAILLE_TERRAIN
        || tower_.y < 0 || tower_.y >= TAILLE_TERRAIN)
        return CASE_IMPOSSIBLE;

    if (target_.x < 0 || target_.x >= TAILLE_TERRAIN
        || target_.y < 0 || target_.y >= TAILLE_TERRAIN)
        return CASE_IMPOSSIBLE;

    if (points_ < 0)
        return VALEUR_INVALIDE;


    // Check of the tower
    tourelle tower = cell_tower->get_tower();
    int dist = distance(tower_, target_);

    if (dist > tower.portee)
        return VALEUR_INVALIDE;

    if (cell_tower->get_type() != CASE_TOURELLE)
        return CASE_VIDE;

    if (cell_tower->get_player() == player_id_)
        return CASE_ADVERSE;

    if (tower.magie < points_)
        return MAGIE_INSUFFISANTE;

    return OK;
}

void ActionShoot::handle_buffer(utils::Buffer& buf)
{
    buf.handle(points_);
    buf.handle(tower_);
    buf.handle(target_);
    buf.handle(player_id_);
}

void ActionShoot::apply_on(GameState* gameState) const
{
    Cell* cell_target = gameState->get_map()->get_cell(target_);
    Cell* cell_tower = gameState->get_map()->get_cell(tower_);

    int wizards_dead = cell_target->wizards_attacked(points_, player_id_);

    cell_tower->set_magic_tower(cell_tower->get_tower().magie - points_);

    // magic gained
    gameState->set_magic(player_id_, gameState->get_magic(player_id_) +
                  (wizards_dead * MAGIE_COMBAT));

}
