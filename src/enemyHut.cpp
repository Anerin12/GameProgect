#include "enemyHut.h"
#include "field.h"
#include "enemy.h"

EnemyHut::EnemyHut(int health, Position position, int counter)
    : Character(health, 0, position), moveCounter(counter) {
        this->move = moveCounter;
    }

Enemy* EnemyHut::generateEnemy()
{
    Position new_pos = this->getPosition();
    new_pos.y++;

    Enemy *enemy = new Enemy(100, 10, new_pos);

    return enemy;
}

bool EnemyHut::update()
{
    move--;
    if (move <= 0)
    {
        move = moveCounter;
        return true;
    }
    
    return false;
}


int EnemyHut::getMove(){return move;}
