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
    Cell* cell_target = st->get_map()->get_cell(target_);

    if (tower_.x < 0 || tower_.x >= TAILLE_TERRAIN
        || tower_.y < 0 || tower_.y >= TAILLE_TERRAIN)
        return CASE_IMPOSSIBLE;

    if (target_.x < 0 || target_.x >= TAILLE_TERRAIN
        || target_.y < 0 || target_.y >= TAILLE_TERRAIN)
        return CASE_IMPOSSIBLE;

    if (points_ < 0)
        return VALEUR_INVALIDE;

    // FIXME: Move checks
    // portee
    // tower exist
    // magic
    // good player
    // life

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
}
