#pragma once
#include "field/field.h"
#include "characters/player.h"
#include "spells/hand.h"
#include "spells/improveSpell.h"
#include "configs/gameConf.h"
#include <vector>
#include <random>
#include <iostream>
#include "saveLoadManager.h"


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
    SaveLoadManager saveManager_;

public:
    Game();

    void gameOver();
    void gameLoop();
    void printSettings();

    //lb3
    void newLevel();
    void improvePlayer();
};
