#ifndef ACTION_ACK_HH_

# define ACTION_ACK_HH_

# include <rules/action.hh>

# include "constant.hh"
# include "game-state.hh"

class ActionAck : public rules::Action<GameState>
{
public:
    ActionAck(int player);

    ActionAck();

    virtual int check(const GameState*) const { return 0; }
    virtual void handle_buffer(utils::Buffer&);

    uint32_t player_id() const
        { return player_; }
    uint32_t id() const
        { return ID_ACTION_ACK; }

protected:
    virtual void apply_on(GameState*) const { }

protected:
    int player_;
    int id_;
};

#endif /* !ACTION_ACK_HH_ */
