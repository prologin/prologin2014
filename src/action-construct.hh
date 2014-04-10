#ifndef ACTION_CONSTRUCT_HH_
# define ACTION_CONSTRUCT_HH_

# include <rules/action.hh>

# include "constant.hh"
# include "game-state.hh"

class ActionConstruct : public rules::Action<GameState>
{
    public:
        ActionConstruct(position pos, int range, int player);
        ActionConstruct();

        virtual int check(const GameState* st) const;
        virtual void handle_buffer(utils::Buffer& buf);
        virtual void apply_on(GameState* gameState) const;

        uint32_t player_id() const { return player_id_; }
        uint32_t id() const { return ID_ACTION_CONSTRUCT; }

    private:
        position pos_;
        int range_;
        int player_id_;
};

#endif /* !ACTION_CONSTRUCT_HH_ */
