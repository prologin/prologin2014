#ifndef ACTION_CREATE_HH
# define ACTION_CREATE_HH

# include <rules/action.hh>

# include "game-state.hh"
# include "constant.hh"

class ActionCreate : public rules::Action<GameState>
{
    public:
        ActionCreate(int nb_wizards, int player);
        ActionCreate();

        virtual int check(const GameState* st) const;
        virtual void handle_buffer(utils::Buffer& buf);
        virtual void apply_on(GameState* gameState) const;

        uint32_t player_id() const { return player_id_; }
        uint32_t id() const { return ID_ACTION_CREATE; }

    private:
        int nb_wizards_;
        int player_id_;
};

#endif // !ACTION_CREATE_HH
