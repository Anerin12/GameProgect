#include "spells/improveSpell.h"
#include "spells/spellCard.h"
#include "spells/trapSpell.h"
#include "spells/callSpell.h"

ImproveSpell::ImproveSpell(){}

void ImproveSpell::use(SpellCard* spell){
    spell->setRadius(spell->getRadius() + 2);
}

void ImproveSpell::use(TrapSpell *spell)
{
    spell->setDamage(spell->getDamage() + 10);
}

void ImproveSpell::use(CallSpell *spell)
{
    spell->setNumAlly(spell->getNumAlly() + 1);
}