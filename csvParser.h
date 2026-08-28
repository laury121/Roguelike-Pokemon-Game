

#ifndef CSVPARSER_H
#define CSVPARSER_H

#include <vector>
#include "pokemon.h"
#include <string>

std::string getDatabasePath();
std::vector<Pokemon> pokemonCSV(const std::string& path); 
std::vector<Move> movesCSV(const std::string& path);
std::vector<Experience> experienceCSV(const std::string& path);
std::vector<TypeNames> typeNamesCSV(const std::string& path);
std::vector<PokemonStats> pokemonStatsCSV(const std::string& path);
std::vector<Stats> statsCSV(const std::string& path);
std::vector<PokemonTypes> pokemonTypesCSV(const std::string& path);
std::vector<PokemonMoves> pokemonMovesCSV(const std::string& path);
std::vector<PokemonSpecies> pokemonSpeciesCSV(const std::string& path);

#endif