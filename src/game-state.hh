#ifndef GAME_STATE_HH
# define GAME_STATE_HH

# include "map.hh"

# include <rules/game-state.hh>
# include <rules/player.hh>

enum action_id
{
    ID_ACTION_CONSTRUCT,
    ID_ACTION_DELETE,
    ID_ACTION_SHOOT,
    ID_ACTION_CREATE,
    ID_ACTION_MOVE,
    ID_ACTION_ATTACK,
    ID_ACTION_CANCEL
};

class GameState : public rules::GameState
{
    public:
        GameState(Map* map, rules::Players_sptr players);
        GameState(const GameState& st);
        virtual rules::GameState* copy() const;
        virtual ~GameState();

        /* turn handling */
        void increment_turn();
        int get_current_turn() const;

        /* Towers */
        bool add_tower(position pos, int player);
        std::vector<tourelle> get_towers(int player);

        /* Magic */
        int get_magic(int player) const;
        void set_magic(int player, int magic);

        std::vector<int> get_opponents(int player) const;

        Map* get_map() const;

        rules::Players_sptr get_players() const
            { return players_; }

    private:
        Map* map_;
        rules::Players_sptr players_;
        std::map<int, rules::Player_sptr> players_ids_;
        int current_turn_;

        std::vector<tourelle> towers_;
        std::map<int, int> magic_;
};

#endif /* !GAME_STATE_HH */
