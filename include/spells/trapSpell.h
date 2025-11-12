#pragma once
#include "spellCard.h"

struct Position;

class TrapSpell : public SpellCard{
    private:
        Position position_;

    public:
        TrapSpell(int damage, Position position);

        Position getPosition();
};