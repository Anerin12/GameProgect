#pragma once
#include "spellCard.h"

class AreaSpell : public SpellCard{
    public:
        AreaSpell(std::string name, std::string description, int damage, int radius);

        //Active methods
        std::pair<std::vector<Position>, int> use(Field &field, Position playerPos) override;
        void setZeroUsed(Field& field);
};  