#include "spells/trapSpell.h"

TrapSpell::TrapSpell(int damage, Position position): SpellCard("Ловушка", "Следи куда идешь", damage, 0), position_(position) {}


Position TrapSpell::getPosition(){
    return position_;
}