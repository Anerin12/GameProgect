#pragma once
#include <memory>      
#include "character.h" 
#include "enemy.h"     

class EnemyHut : public Character{
    private:
        int moveCounter;
        int move;


    public:
        //Constructor/Destructor
        EnemyHut(int health, Position position, int counter = 5);

        //Active methods
        bool update();
        std::unique_ptr<Enemy> generateEnemy();


        // getters
        int getMove();
};      
