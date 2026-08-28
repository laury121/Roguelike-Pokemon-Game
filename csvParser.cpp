
#include "csvParser.h"
#include <fstream>
#include <sstream>
#include <climits>
#include <iostream>
#include <filesystem>
#include <cstdlib>

std::string getDatabasePath() {
    std::string path1 = "/share/cs327/pokedex";

    if (std::filesystem::exists(path1)) {
        return path1 + "/pokedex/data/csv";
    }

    char* home = std::getenv("HOME");
    if (home) {
        std::string path2 = std::string(home) + "/.poke327/pokedex";
        if (std::filesystem::exists(path2)) {
            return path2 + "/pokedex/data/csv";
        }
    }

    throw std::runtime_error("Database not found");
}

std::vector<std::vector<std::string>> parseCSV(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + path);
    }

    std::vector<std::vector<std::string>> data;
    std::string line;

    std::getline(file, line); // Skip header line
    while (std::getline(file, line)) {
        std::vector<std::string> row;
        std::stringstream ss(line);
        std::string item;

        while (std::getline(ss, item, ',')) {
            row.push_back(item);
        }
        data.push_back(row);
    }

    return data;
}


std::vector<Pokemon> pokemonCSV(const std::string& path) {
    std::vector<Pokemon> pokemonList;
    auto data = parseCSV(path);

    for (const auto& row : data) {
        Pokemon p;
        p.id = (row.size() > 0 && !row[0].empty()) ? std::stoi(row[0]) : std::numeric_limits<int>::max();
        p.identifier = (row.size() > 1 && !row[1].empty()) ? row[1] : "";
        p.species_id = (row.size() > 2 && !row[2].empty()) ? std::stoi(row[2]) : std::numeric_limits<int>::max();
        p.height = (row.size() > 3 && !row[3].empty()) ? std::stoi(row[3]) : std::numeric_limits<int>::max();
        p.weight = (row.size() > 4 && !row[4].empty()) ? std::stoi(row[4]) : std::numeric_limits<int>::max();
        p.base_experience = (row.size() > 5 && !row[5].empty()) ? std::stoi(row[5]) : std::numeric_limits<int>::max();
        p.order = (row.size() > 6 && !row[6].empty()) ? std::stoi(row[6]) : std::numeric_limits<int>::max();
        p.is_default = (row.size() > 7 && !row[7].empty()) ? std::stoi(row[7]) : std::numeric_limits<int>::max();

        pokemonList.push_back(p);
    }

    return pokemonList;
}

std::vector<Move> movesCSV(const std::string& path) {
    std::vector<Move> moveList;
    auto data = parseCSV(path);

    for (const auto& row : data) {
        Move m;
        m.id = (row.size() > 0 && !row[0].empty()) ? std::stoi(row[0]) : std::numeric_limits<int>::max();
        m.identifier = (row.size() > 1 && !row[1].empty()) ? row[1] : "";
        m.generation_id = (row.size() > 2 && !row[2].empty()) ? std::stoi(row[2]) : std::numeric_limits<int>::max();
        m.type_id = (row.size() > 3 && !row[3].empty()) ? std::stoi(row[3]) : std::numeric_limits<int>::max();
        m.power = (row.size() > 4 && !row[4].empty()) ? std::stoi(row[4]) : std::numeric_limits<int>::max();
        m.pp = (row.size() > 5 && !row[5].empty()) ? std::stoi(row[5]) : std::numeric_limits<int>::max();
        m.accuracy = (row.size() > 6 && !row[6].empty()) ? std::stoi(row[6]) : std::numeric_limits<int>::max();
        m.priority = (row.size() > 7 && !row[7].empty()) ? std::stoi(row[7]) : std::numeric_limits<int>::max();
        m.target_id = (row.size() > 8 && !row[8].empty()) ? std::stoi(row[8]) : std::numeric_limits<int>::max();
        m.damage_class_id = (row.size() > 9 && !row[9].empty()) ? std::stoi(row[9]) : std::numeric_limits<int>::max();
        m.effect_id = (row.size() > 10 && !row[10].empty()) ? std::stoi(row[10]) : std::numeric_limits<int>::max();
        m.effect_chance = (row.size() > 11 && !row[11].empty()) ? std::stoi(row[11]) : std::numeric_limits<int>::max();
        m.contest_type_id = (row.size() > 12 && !row[12].empty()) ? std::stoi(row[12]) : std::numeric_limits<int>::max();
        m.contest_effect_id = (row.size() > 13 && !row[13].empty()) ? std::stoi(row[13]) : std::numeric_limits<int>::max();
        m.super_contest_effect_id = (row.size() > 14 && !row[14].empty()) ? std::stoi(row[14]) : std::numeric_limits<int>::max();

        moveList.push_back(m);
    }

    return moveList;
}

std::vector<PokemonMoves> pokemonMovesCSV(const std::string& path) {
    std::vector<PokemonMoves> pokemonMovesList;
    auto data = parseCSV(path);

    for (const auto& row : data) {
        PokemonMoves pm;
        pm.pokemon_id = (row.size() > 0 && !row[0].empty()) ? std::stoi(row[0]) : std::numeric_limits<int>::max();
        pm.version_group_id = (row.size() > 1 && !row[1].empty()) ? std::stoi(row[1]) : std::numeric_limits<int>::max();
        pm.move_id = (row.size() > 2 && !row[2].empty()) ? std::stoi(row[2]) : std::numeric_limits<int>::max();
        pm.pokemon_move_method_id = (row.size() > 3 && !row[3].empty()) ? std::stoi(row[3]) : std::numeric_limits<int>::max();
        pm.level = (row.size() > 4 && !row[4].empty()) ? std::stoi(row[4]) : std::numeric_limits<int>::max();
        pm.order = (row.size() > 5 && !row[5].empty()) ? std::stoi(row[5]) : std::numeric_limits<int>::max();

        pokemonMovesList.push_back(pm);
    }

    return pokemonMovesList;
}

std::vector<PokemonSpecies> pokemonSpeciesCSV(const std::string& path) {
    std::vector<PokemonSpecies> pokemonSpeciesList;
    auto data = parseCSV(path);

    for (const auto& row : data) {
        PokemonSpecies ps;
        ps.id = (row.size() > 0 && !row[0].empty()) ? std::stoi(row[0]) : std::numeric_limits<int>::max();
        ps.identifier = (row.size() > 1 && !row[1].empty()) ? row[1] : "";
        ps.generation_id = (row.size() > 2 && !row[2].empty()) ? std::stoi(row[2]) : std::numeric_limits<int>::max();
        ps.evolves_from_species_id = (row.size() > 3 && !row[3].empty()) ? std::stoi(row[3]) : std::numeric_limits<int>::max();
        ps.evolution_chain_id = (row.size() > 4 && !row[4].empty()) ? std::stoi(row[4]) : std::numeric_limits<int>::max();
        ps.color_id = (row.size() > 5 && !row[5].empty()) ? std::stoi(row[5]) : std::numeric_limits<int>::max();
        ps.shape_id = (row.size() > 6 && !row[6].empty()) ? std::stoi(row[6]) : std::numeric_limits<int>::max();
        ps.habitat_id = (row.size() > 7 && !row[7].empty()) ? std::stoi(row[7]) : std::numeric_limits<int>::max();
        ps.gender_rate = (row.size() > 8 && !row[8].empty()) ? std::stoi(row[8]) : std::numeric_limits<int>::max();
        ps.capture_rate = (row.size() > 9 && !row[9].empty()) ? std::stoi(row[9]) : std::numeric_limits<int>::max();
        ps.base_happiness = (row.size() > 10 && !row[10].empty()) ? std::stoi(row[10]) : std::numeric_limits<int>::max();
        ps.is_baby = (row.size() > 11 && !row[11].empty()) ? std::stoi(row[11]) : std::numeric_limits<int>::max();
        ps.hatch_counter = (row.size() > 12 && !row[12].empty()) ? std::stoi(row[12]) : std::numeric_limits<int>::max();
        ps.has_gender_differences = (row.size() > 13 && !row[13].empty()) ? std::stoi(row[13]) : std::numeric_limits<int>::max();
        ps.growth_rate_id = (row.size() > 14 && !row[14].empty()) ? std::stoi(row[14]) : std::numeric_limits<int>::max();
        ps.forms_switchable = (row.size() > 15 && !row[15].empty()) ? std::stoi(row[15]) : std::numeric_limits<int>::max();
        ps.is_legendary = (row.size() > 16 && !row[16].empty()) ? std::stoi(row[16]) : std::numeric_limits<int>::max();
        ps.is_mythical = (row.size() > 17 && !row[17].empty()) ? std::stoi(row[17]) : std::numeric_limits<int>::max();
        ps.order = (row.size() > 18 && !row[18].empty()) ? std::stoi(row[18]) : std::numeric_limits<int>::max();
        ps.conquest_order = (row.size() > 19 && !row[19].empty()) ? std::stoi(row[19]) : std::numeric_limits<int>::max();

        pokemonSpeciesList.push_back(ps);
    }

    return pokemonSpeciesList;
}

std::vector<Experience> experienceCSV(const std::string& path) {
    std::vector<Experience> experienceList;
    auto data = parseCSV(path);

    for (const auto& row : data) {
        Experience e;
        e.growth_rate_id = (row.size() > 0 && !row[0].empty()) ? std::stoi(row[0]) : std::numeric_limits<int>::max();
        e.level = (row.size() > 1 && !row[1].empty()) ? std::stoi(row[1]) : std::numeric_limits<int>::max();
        e.experience = (row.size() > 2 && !row[2].empty()) ? std::stoi(row[2]) : std::numeric_limits<int>::max();

        experienceList.push_back(e);
    }

    return experienceList;
}

std::vector<TypeNames> typeNamesCSV(const std::string& path) {
    std::vector<TypeNames> typeNamesList;
    auto data = parseCSV(path);

    for (const auto& row : data) {
        TypeNames t;
        t.type_id = (row.size() > 0 && !row[0].empty()) ? std::stoi(row[0]) : std::numeric_limits<int>::max();
        t.local_language_id = (row.size() > 1 && !row[1].empty()) ? std::stoi(row[1]) : std::numeric_limits<int>::max();
        t.name = (row.size() > 2 && !row[2].empty())    ? row[2] : "";

        typeNamesList.push_back(t);
    }

    return typeNamesList;
}

std::vector<PokemonStats> pokemonStatsCSV(const std::string& path) {
    std::vector<PokemonStats> pokemonStatsList;
    auto data = parseCSV(path);

    for (const auto& row : data) {
        PokemonStats ps;
        ps.pokemon_id = (row.size() > 0 && !row[0].empty()) ? std::stoi(row[0]) : std::numeric_limits<int>::max();
        ps.stat_id = (row.size() > 1 && !row[1].empty()) ? std::stoi(row[1]) : std::numeric_limits<int>::max();
        ps.base_stat = (row.size() > 2 && !row[2].empty()) ? std::stoi(row[2]) : std::numeric_limits<int>::max();
        ps.effort = (row.size() > 3 && !row[3].empty()) ? std::stoi(row[3]) : std::numeric_limits<int>::max();

        pokemonStatsList.push_back(ps);
    }

    return pokemonStatsList;
}

std::vector<Stats> statsCSV(const std::string& path) {
    std::vector<Stats> statsList;
    auto data = parseCSV(path);

    for (const auto& row : data) {
        Stats s;
        s.id = (row.size() > 0 && !row[0].empty()) ? std::stoi(row[0]) : std::numeric_limits<int>::max();
        s.damage_class_id = (row.size() > 1 && !row[1].empty()) ? std::stoi(row[1]) : std::numeric_limits<int>::max();
        s.identifier = (row.size() > 2 && !row[2].empty()) ? row[2] : "";
        s.is_battle_only = (row.size() > 3 && !row[3].empty()) ? std::stoi(row[3]) : std::numeric_limits<int>::max();
        s.game_index = (row.size() > 4 && !row[4].empty()) ? std::stoi(row[4]) : std::numeric_limits<int>::max();

        statsList.push_back(s);
    }

    return statsList;
}

std::vector<PokemonTypes> pokemonTypesCSV(const std::string& path) {
    std::vector<PokemonTypes> pokemonTypesList;
    auto data = parseCSV(path);

    for (const auto& row : data) {
        PokemonTypes pt;
        pt.pokemon_id = (row.size() > 0 && !row[0].empty()) ? std::stoi(row[0]) : std::numeric_limits<int>::max();
        pt.type_id = (row.size() > 1 && !row[1].empty()) ? std::stoi(row[1]) : std::numeric_limits<int>::max();
        pt.slot = (row.size() > 2 && !row[2].empty()) ? std::stoi(row[2]) : std::numeric_limits<int>::max();

        pokemonTypesList.push_back(pt);
    }

    return pokemonTypesList;
}