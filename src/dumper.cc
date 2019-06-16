/*
** Copyright (c) 2014 Mélanie Godard <melanie.godard@prologin.org>
** Copyright (c) 2014 Antoine Pietri <antoine.pietri@prologin.org>
** Copyright (c) 2014 Association Prologin <info@prologin.org>
**
** prologin2014 is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** prologin2014 is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with prologin2014.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <cstdlib>
#include <cstring>
#include <sstream>

#include <rules/action.hh>
#include <rules/actions.hh>
#include <utils/buffer.hh>

#include "cell.hh"
#include "dumper.hh"
#include "map.hh"

/* Put some binary content as a JSON string in the output stream. */
static void dump_binary(std::ostream& ss, const uint8_t bytes[], unsigned size)
{
    const char* hextable = "0123456789ABCDEF";

    ss << "\"";
    for (unsigned i = 0; i < size; ++i)
    {
        if (bytes[i] == '"')
            ss << "\\\"";
        else if (bytes[i] == '\\')
            ss << "\\\\";
        else if (0x20 <= bytes[i] && bytes[i] <= 0x7e)
            ss << (char)bytes[i];
        else
            ss << "\\u00" << hextable[bytes[i] >> 4]
               << hextable[bytes[i] & 0x0f];
    }
    ss << "\"";
}

static void dump_players(std::ostream& ss, const GameState& st)
{
    auto& players = st.get_players()->players;
    bool first_player = true;

    ss << "{";
    for (unsigned i = 0; i < players.size(); ++i)
    {
        const int id = players[i]->id;

        if (players[i]->type != rules::PLAYER)
            continue;

        if (first_player)
            first_player = false;
        else
            ss << ", ";
        ss << "\"" << id << "\": {"
           << "\"name\": ";
        dump_binary(ss,
                    reinterpret_cast<const uint8_t*>(players[i]->name.c_str()),
                    players[i]->name.size());
        ss << ", "
           << "\"alive\": ";
        if (!st.has_lost(id))
            ss << "true";
        else
            ss << "false";
        ss << ", "
           << "\"magic\": " << st.get_magic(id);
        ss << ", "
           << "\"score\": " << players[i]->score << "}";
    }
    ss << "}";
}

static void dump_map(std::ostream& ss, const GameState& st)
{
    ss << "{"
       << "\"cells\": [";
    auto& map = *st.get_map();
    bool first_cell = true;
    std::vector<tourelle> towers;

    for (int y = 0; y < TAILLE_TERRAIN; ++y)
        for (int x = 0; x < TAILLE_TERRAIN; ++x)
        {
            const Cell& c = *(map.get_cell((position){x, y}));

            if (c.get_player() != -1 || c.get_type() != CASE_SIMPLE)
            {
                if (c.get_type() == CASE_TOURELLE)
                    towers.push_back(c.get_tower());

                if (first_cell)
                    first_cell = false;
                else
                    ss << ", ";
                ss << "{"
                   << "\"x\": " << x << ", "
                   << "\"y\": " << y << ", "
                   << "\"type\": " << c.get_type() << ", "
                   << "\"object-player\": " << c.get_player() << ", "
                   << "\"wizards\" : " << c.get_nb_wizards_total() << ", "
                   << "\"wizards-player\": " << c.get_wizards_player() << "}";
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
           << "\"attack\": " << tower.attaque << "}";
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

static void dump_phase(std::ostream& ss, const GameState& st)
{
    static const char* phase_names[] = {
        "construction",
        "move",
        "shoot",
        "siege",
    };
    ss << '"' << phase_names[st.getPhase()] << '"';
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
    ss << "\"turn\": [" << st.get_current_round() << ", " << MAX_TOUR << "], ";

    ss << "\"phase\": ";
    dump_phase(ss, st);

    ss << ", "
       << "\"players\": ";
    dump_players(ss, st);

    ss << ", "
       << "\"map\": ";
    dump_map(ss, st);

    ss << ", "
       << "\"actions\": ";
    dump_actions(ss, acts);

    ss << "}";

    /* Use malloc(), since the caller will use free(). */
    char* result = (char*)malloc(sizeof(char) * ss.str().size() + 1);

    strcpy(result, ss.str().c_str());
    return result;
}
