#ifndef ACTION_DELETE_HH
# define ACTION_DELETE_HH

# include <rules/action.hh>

# include "game-state.hh"
# include "constant.hh"

class ActionDelete : public rules::Action<GameState>
{
    public:
        ActionDelete(position pos, int player);
        ActionDelete();

        virtual int check(const GameState* st) const;
        virtual void handle_buffer(utils::Buffer& buf);
        virtual void apply_on(GameState* gameState) const;

        uint32_t player_id() const { return player_id_; }
        uint32_t id() const { return ID_ACTION_DELETE; }

    private:
        position position_;
        int player_id_;
};

#endif // !ACTION_DELETE_HH
