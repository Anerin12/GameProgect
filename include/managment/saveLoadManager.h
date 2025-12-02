#include <chrono>
#include <format>
#include <filesystem>
#include "enemyManager.h"
#include "spellManager.h"
#include <fstream>
#include <format>


class SaveLoadManager {

    public:
        SaveLoadManager();
        void save(SpellManager &spellManager_, EnemyManager &enemyManager_, Field &field_, Player &player_, int level_);
        void load(SpellManager &spellManager_, EnemyManager &enemyManager_, Field &field_, Player &player_, int &level_);
};