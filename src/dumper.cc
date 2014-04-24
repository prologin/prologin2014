#include <cstring>
#include <cstdlib>
#include <sstream>

#include <rules/action.hh>
#include <rules/actions.hh>
#include <utils/buffer.hh>

#include "map.hh"
#include "cell.hh"
#include "dumper.hh"

/* Put some binary content as a JSON string in the output stream. */
static void dump_binary(
    std::ostream& ss, const uint8_t bytes[], unsigned size
)
{
    const char* hextable = "0123456789ABCDEF";

    ss << "\"";
    for (unsigned i = 0; i < size; ++i)
        ss << "\\u00"
           << hextable[bytes[i] >> 4]
           << hextable[bytes[i] & 0x0f];
    ss << "\"";
}

static void dump_players(std::ostream& ss, const GameState& st)
{
    auto& players = st.get_players()->players;
    bool first_player = true;

    ss << "{";
    for (unsigned i = 0; i < players.size(); ++i)
    {
        if (players[i]->type != rules::PLAYER)
            continue;

        if (first_player)
            first_player = false;
        else
            ss << ", ";
        ss << "\"" << players[i]->id << "\": {"
           << "\"name\": ";
        dump_binary(
            ss,
            reinterpret_cast<const uint8_t *>(players[i]->name.c_str()),
            players[i]->name.size()
        );
        ss << ", "
           << "\"magic\": " << st.get_magic(i);
        ss << ", "
           << "\"score\": " << players[i]->score
           << "}";
    }
    ss << "}";
}

static void dump_map(std::ostream& ss, const GameState& st)
{
    ss << "{"
       << "\"width\": " << TAILLE_TERRAIN << ", "
       << "\"height\": " << TAILLE_TERRAIN << ", "
       << "\"cells\": [";
    auto& map = *st.get_map();
    bool first_cell = true;
    std::vector<tourelle> towers;

    for (int y = 0; y < TAILLE_TERRAIN; ++y)
        for (int x = 0; x < TAILLE_TERRAIN; ++x)
        {
            const Cell& c = *(map.get_cell((position) {x, y}));

            if (c.get_player() != -1 || c.get_type() != CASE_SIMPLE)
            {
                if (c.get_type() == CASE_TOURELLE)
                    towers.push_back(*c.get_tower());

                if (first_cell)
                    first_cell = false;
                else
                    ss << ", ";
                ss << "{"
                   << "\"x\": " << x << ", "
                   << "\"y\": " << y << ", "
                   << "\"type\": " << c.get_type() << ", "
                   << "\"player\": " << c.get_player()
                   << "\"wizards\" : " << c.get_nb_wizards_total()
                   << "}";
            }
        }
    ss << "], ";

    bool first_tower = true;

    // dump towers
    ss << "\"towers\": [";
    for (auto tower : towers)
    {
        if (first_tower)
            first_tower = false;
        else
            ss << ", ";

        ss << "{"
           << "\"x\": " << tower.pos.x << ", "
           << "\"y\": " << tower.pos.y << ", "
           << "\"player\": " << tower.joueur << ", "
           << "\"scope\": " << tower.portee << ", "
           << "\"life\": " << tower.vie << ", "
           << "\"attack\": " << tower.attaque << ", "
           << "}";
    }

    ss << "]"
       << "}";
}

static void dump_actions(std::ostream& ss, rules::Actions& acts)
{
    utils::Buffer buf;
    acts.handle_buffer(buf);
    dump_binary(ss, buf.data(), buf.size());
}

/*
 * Return a JSON representation of the given GameState, including the given
 * list of actions.
 * The returned string is heap-allocated and must be delete'd by the caller.
 * The JSON tree fits in one line (i.e. it doesn't contain a new line
 * character), making it easy to maintain a collection of dumps in a text file.
 */ 
char* dump_game_state(const GameState& st, rules::Actions& acts)
{
    std::stringstream ss;

    ss << "{";

    // - "turn": [current turn, number of turns]
    ss << "\"turn\": ["
        << st.get_current_turn() << ", "
        << MAX_TOUR
        << "], ";

    ss << "\"players\": ";
    dump_players(ss, st);

    ss << ", "
        << "\"map\": ";
    dump_map(ss, st);

    ss << ", "
        << "\"actions\": ";
    dump_actions(ss, acts);

    ss << "}";

    /* Use malloc(), since the caller will use free(). */
    char* result = (char*) malloc(sizeof (char) * ss.str().size() + 1);

    strcpy(result, ss.str().c_str());
    return result;
}

