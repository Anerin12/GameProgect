#include "spells/callSpell.h"

CallSpell::CallSpell(int numAlly) : SpellCard("Заклинание призыва", "Призывает союзника", 0, 1), numAlly_(numAlly){}

std::pair<std::vector<Position>, int> CallSpell::use(Field &field, Position playerPos){
    Position resPos = {-1,-1};
    for (int y = 0; y < this->getRadius()+1; y++){
        for (int x = 0; x < this->getRadius()+1; x++){
            Position newPos = {playerPos.x+x, playerPos.y+y};
            if(field.isFree(newPos)){
                resPos = newPos;
                break;
            }
        }
    }
    return {{resPos}, 0};
}

int CallSpell::getNumAlly(){return numAlly_;}

void CallSpell::setNumAlly(int numAlly){numAlly_ = numAlly;}