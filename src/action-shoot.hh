#ifndef ACTION_SHOOT_HH
# define ACTION_SHOOT_HH

# include <rules/action.hh>

# include "game-state.hh"
# include "constant.hh"

class ActionShoot : public rules::Action<GameState>
{
    public:
        ActionShoot(int points, position tower, position target, int player);
        ActionShoot();

        virtual int check(const GameState* st) const;
        virtual void handle_buffer(utils::Buffer& buf);
        virtual void apply_on(GameState* gameState) const;

        uint32_t player_id() const { return player_id_; }
        uint32_t id() const { return ID_ACTION_SHOOT; }
    private:
        int points_;
        position tower_;
        position target_;
        int player_id_;
};

#endif // !ACTION_SHOOT_HH
