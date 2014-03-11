#ifndef GAME_STATE_HH_
# define GAME_STATE_HH_

# include <rules/game-state.hh>

class GameState : public rules::GameState
{
    public:
        GameState(rules::Players_sptr players);
        GameState(const GameState& st);
        virtual rules::GameState* copy() const;
        ~GameState();

        void increment_turn();
        int get_current_turn() const;
        
    private:
};

#endif /* !GAME_STATE_HH */
