#pragma once
#include "field.h"
#include "player.h"
#include "enemy.h"
#include "enemyHut.h"
#include "heand.h"
#include "tower.h"
#include "ally.h"
#include "spells/improveSpell.h"
#include <vector>
#include <random>
#include <iostream>

class Game
{
private:
    Player player_;
    Field field_;
    std::vector<Enemy*> enemies_;
    std::vector<EnemyHut *> enemyHuts_;
    bool isRuning_ = true;


    //lb2
    std::vector<Tower *> towers_;
    std::vector<Ally *> allies_;
    

public:
    Game();

    void gameOver();
    void gameLoop();
    void generateEnemy(int numEnemy);
    void generateEnemyHut(int numEnemyHut);
    void enemiesManager();
    void enemyHutsManager();
    void printSettings();

    //lb2
    void spellManager();
    void towerManager();
    void generateTower(int numTower);
    void callSpellManager();
    void allyManager();
    void improveManager();
};
