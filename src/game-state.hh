#ifndef GAME_STATE_HH
# define GAME_STATE_HH

# include <rules/game-state.hh>
# include <rules/player.hh>

#include <unordered_set>
#include <unordered_map>

# include "map.hh"

enum game_phase
{
    PHASE_CONSTRUCTION,
    PHASE_SPAWN,
    PHASE_MOVE,
    PHASE_SHOOT,
    PHASE_ATTACK_TOWER
};

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

        void setPhase(game_phase phase);
        game_phase getPhase() const;

        /* Towers */
        std::vector<tourelle> get_towers(int player);

        /* Magic */
        int get_magic(int player) const;
        void set_magic(int player, int magic);

        std::vector<int> get_opponents(int player) const;

        Map* get_map() const;

        rules::Players_sptr get_players() const
            { return players_; }

        std::map<int, rules::Player_sptr> get_players_ids() const
            { return players_ids_; }
        // Function for the score
        int get_player_artefact();
        int get_nb_fontains(int player_id);

        // Resolve fights
        void resolve_fights();

    private:
        Map* map_;
        rules::Players_sptr players_;
        std::map<int, rules::Player_sptr> players_ids_;

        std::map<int, int> magic_;

        game_phase game_phase_;
        int current_turn_;
};

#endif /* !GAME_STATE_HH */
