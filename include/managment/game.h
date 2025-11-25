#pragma once
#include "field/field.h"
#include "characters/player.h"
#include "spells/hand.h"
#include "spells/improveSpell.h"
#include "configs/gameConf.h"
#include <vector>
#include <random>
#include <iostream>
#include "enemyManager.h"
#include "spellManager.h"
#include <fstream>
#include <format>
#include <filesystem>

class Game
{
private:
    std::unique_ptr<Player> player_;
    Field field_;
    bool isRuning_ = true;
    int level_;
    EnemyManager enemyManager_;
    SpellManager spellManager_;
    conf_t gameConf;

public:
    Game();

    void gameOver();
    void gameLoop();
    void printSettings();

    //lb3
    void newLevel();
    void improvePlayer();
    void improveEnemy();
    void clearCharacter();
    void saveGame();
    void loadGame();
};
