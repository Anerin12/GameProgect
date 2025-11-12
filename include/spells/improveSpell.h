#pragma once

class SpellCard;
class TrapSpell;
class CallSpell;

class ImproveSpell{
    public:
        ImproveSpell();

        void use(SpellCard* spell);
        void use(TrapSpell *spell);
        void use(CallSpell *spell);
};