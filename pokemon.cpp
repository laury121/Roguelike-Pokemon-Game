


#include "pokemon.h"
#include "maps.h"
#include "characters.h"
#include "battle.h"

extern std::vector<Pokemon> pokemonList;
extern std::vector<PokemonMoves> pokemonMovesList;
extern std::vector<Move> moveList;
extern std::vector<PokemonStats> pokemonStatsList;


void tryEncounter(Map* m, Character* pc, int worldX, int worldY) {
    if (rand() % 100 >= 10) return; // 10% chance

    GeneratedPokemon wild = generatePokemon(worldX, worldY);

    //create temporary wild trainer
    Character* wildTrainer = new Character();
    wildTrainer->party.push_back(wild);
    wildTrainer->activeIndex = 0;

    //encounter message
    clear();
    mvprintw(HEIGHT + 3, 0, "A wild %s appeared!",
        wild.base->identifier.c_str());
    mvprintw(HEIGHT + 4, 0, "Level: %d", wild.level);
    mvprintw(HEIGHT + 6, 0, "Press any key to battle!");
    refresh();
    getch();

    //start battle
    Battle b(pc, wildTrainer, true);
    b.start();

    //Clean up
    delete wildTrainer;
}

int calculateLevel(int worldX, int worldY) {
    int distance = abs(worldX - 200) + abs(worldY - 200);
    int minLevel, maxLevel;
    if (distance <= 200) {
        minLevel = 1;
        maxLevel = std::max(1, distance / 2);
    } else {
        minLevel = (distance - 200) / 2;
        maxLevel = 100;
    }
    return minLevel + rand() % (maxLevel - minLevel + 1);
}

std::vector<const Move*> getMoves(int pokemonId, int level) {
    std::vector<const Move*> validMoves;
    for (const auto& pm : pokemonMovesList) {
        if (pm.pokemon_id == pokemonId && pm.level <= level && pm.pokemon_move_method_id == 1) { //level-up moves only
            for (const auto& m : moveList) {
                if (m.id == pm.move_id) {
                    validMoves.push_back(&m);
                    break;
                }
            }
        }
    }
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(validMoves.begin(), validMoves.end(), g); //randomize move order
    std::vector<const Move*> selectedMoves;
    for (size_t i = 0; i < validMoves.size() && i < 2; i++) {
        selectedMoves.push_back(validMoves[i]);
    }
    return selectedMoves;
}

void getBaseStats(int pokemonId, int& hp, int& attack, int& defense, int& spAttack, int& spDefense, int& speed) {
    for (const auto& ps : pokemonStatsList) {
        if (ps.pokemon_id == pokemonId) {
            switch (ps.stat_id) {
                case 1: hp = ps.base_stat; break;
                case 2: attack = ps.base_stat; break;
                case 3: defense = ps.base_stat; break;
                case 4: spAttack = ps.base_stat; break;
                case 5: spDefense = ps.base_stat; break;
                case 6: speed = ps.base_stat; break;
                
            }
        }
    }
}

GeneratedPokemon generatePokemon(int worldX, int worldY) {
    GeneratedPokemon p;
    const Pokemon& base = pokemonList[rand() % pokemonList.size()]; //random base pokemon
    p.base = &base;
    p.id = base.id; //use the base pokemon's ID
    p.level = calculateLevel(worldX, worldY);

    //base stats
    int baseHp = 0, baseAttack = 0, baseDefense = 0, baseSpAttack = 0, baseSpDefense = 0, baseSpeed = 0;
    getBaseStats(p.id, baseHp, baseAttack, baseDefense, baseSpAttack, baseSpDefense, baseSpeed);
   
    //ivs
    p.ivHp = rand() % 16;
    p.ivAttack = rand() % 16;
    p.ivDefense = rand() % 16;
    p.ivSpeed = rand() % 16;
    p.ivSpAttack = rand() % 16;
    p.ivSpDefense = rand() % 16;

    //final stats
    p.hp = ((baseHp + p.ivHp) * 2 * p.level) / 100 + p.level + 10;
    p.attack = ((baseAttack + p.ivAttack) * 2 * p.level) / 100 + 5;
    p.defense = ((baseDefense + p.ivDefense) * 2 * p.level) / 100 + 5;
    p.speed = ((baseSpeed + p.ivSpeed) * 2 * p.level) / 100 + 5;
    p.spAttack = ((baseSpAttack + p.ivSpAttack) * 2 * p.level) / 100 + 5;
    p.spDefense = ((baseSpDefense + p.ivSpDefense) * 2 * p.level) / 100 + 5;

    //moves
    p.moves = getMoves(p.id, p.level);
    while (p.moves.empty() && p.level < 100) { //ensure at least 1 move
        p.level++;
        p.moves = getMoves(p.id, p.level);
    }
    //extras
    p.isShiny = (rand() % 8192 == 0); //1 in 8192 chance to be shiny
    p.isMale = (rand() % 2 == 0); //50% chance

    return p;
}