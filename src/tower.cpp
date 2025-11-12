#include "tower.h"

Tower::Tower(int health, Position position): Character(health, 0, position){
    spell_ = new DirectSpell("Огненный шар", "", 10, 2);
}

DirectSpell* Tower::attack(){
    return spell_;
}