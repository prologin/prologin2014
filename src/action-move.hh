#ifndef ACTION_MOVE_HH
# define ACTION_MOVE_HH

# include <rules/action.hh>

# include "game-state.hh"
# include "constant.hh"

class ActionMove : public rules::Action<GameState>
{
    public:
        ActionMove(position start, position dest, int nb_wizards, int player);
        ActionMove();

        virtual int check(const GameState* st) const;
        virtual void handle_buffer(utils::Buffer& buf);
        virtual void apply_on(GameState* gameState) const;

        uint32_t player_id() const { return player_id_; }
        uint32_t id() const { return ID_ACTION_MOVE; }
    private:
        position start_;
        position dest_;
        int nb_wizards_;
        int player_id_;
};

#endif // !ACTION_MOVE_HH
