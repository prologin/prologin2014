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

        /* Accessors */
        rules::Players_sptr get_players() const
        { return players_; }
        
    private:
        std::map<int, rules::Players_sptr> player_ids_;

};

#endif /* !GAME_STATE_HH */
