#pragma once
#include "spellCard.h"

class DirectSpell : public SpellCard
{
public:
    DirectSpell(std::string name, std::string description, int damage, int radius);

    // Active methods
    std::pair<std::vector<Position>, int> use(Field &field, Position playerPos) override;
};