#ifndef ACTION_ATTACK_HH
# define ACTION_ATTACK_HH

# include <rules/action.hh>

# include "game-state.hh"
# include "constant.hh"

class ActionAttack : public rules::Action<GameState>
{
    public:
        ActionAttack(position pos, position target, int player);
        ActionAttack();

        virtual int check(const GameState* st) const;
        virtual void handle_buffer(utils::Buffer& buf);
        virtual void apply_on(GameState* st) const;

        uint32_t player_id() const { return player_id_; }
        uint32_t id() const { return ID_ACTION_ATTACK; }

    private:
        position pos_;
        position target_;
        int player_id_;
};

#endif // !ACTION_ATTACK_HH
