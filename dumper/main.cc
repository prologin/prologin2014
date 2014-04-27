#include <cstdlib>
#include <iostream>
#include <fstream>


extern "C"
{
    char* api_get_dump();
    void partie_debut();
    void partie_fin();
}

static std::ofstream fs;

void partie_debut()
{
    std::string path = "dump.json";
    if (getenv("DUMP_PATH"))
        path = getenv("DUMP_PATH");
    fs.open(path.c_str());
}

void dump_line()
{
    char* json_dump = api_get_dump();
    fs << json_dump << std::endl;
    free(json_dump);
}

extern "C"
{
    auto phase_construction = dump_line;
    auto phase_deplacement = dump_line;
    auto phase_tirs = dump_line;
    auto phase_siege = dump_line;
}

void partie_fin()
{
    fs.close();
}
