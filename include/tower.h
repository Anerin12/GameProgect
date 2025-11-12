#include "character.h"
#include "spells/directSpell.h"

class Player;

class Tower : public Character {
    private:
        DirectSpell *spell_;

    public:
        Tower(int health, Position position);

        DirectSpell* attack();
};