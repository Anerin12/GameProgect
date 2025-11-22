#pragma once
#include "field.h"
#include "player.h"
#include "enemy.h"
#include "enemyHut.h"
#include "heand.h"
#include "tower.h"
#include "ally.h"
#include "spells/improveSpell.h"
#include "config.h"
#include <vector>
#include <random>
#include <iostream>
#include <fstream>
#include <format>
#include <filesystem>

class Game
{
private:
    std::unique_ptr<Player> player_;
    Field field_;
    std::vector<std::unique_ptr<Enemy>> enemies_;
    std::vector<std::unique_ptr<EnemyHut>> enemyHuts_;
    bool isRuning_ = true;
    int level_;


    //lb2
    std::vector <std::unique_ptr<Tower >> towers_;
    std::vector <std::unique_ptr<Ally>> allies_;

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

    //lb3
    void newLevel();
    void improvePlayer();
    void improveEnemy();
    void clearCharacter();
    void saveGame();
    void loadGame();
};
