#ifndef GAME_STATE_HH
# define GAME_STATE_HH

# include "map.hh"

# include <rules/game-state.hh>
# include <rules/player.hh>

class GameState : public rules::GameState
{
    public:
        GameState(Map* map, rules::Players_sptr players);
        GameState(const GameState& st);
        virtual rules::GameState* copy() const;
        ~GameState();

        void increment_turn();
        int get_current_turn() const;

        /* Accessors */
        rules::Players_sptr get_players() const
        { return players_; }

        std::vector<int> get_opponents(int player) const;
        Map* get_map() const;

    private:
        Map* map_;
        rules::Players_sptr players_;
        std::map<int, rules::Players_sptr> players_ids_;
        int current_turn_;

};

#endif /* !GAME_STATE_HH */
