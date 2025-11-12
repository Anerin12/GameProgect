#pragma once
#include "character.h"

class Player;

class Enemy : public Character{
    public:
        //Constructor/Destructor
        Enemy(int health, int damage, Position position);

        //Active methods
        void attack(Player& player);
};