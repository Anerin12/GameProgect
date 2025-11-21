#pragma once
#include "spells/spellCard.h"
#include <random>

class Heand {
    private:

    std::vector<SpellCard*> spells_;
    int size_;

    public:
        Heand(int size);
        ~Heand();

        void addSpell(SpellCard* spell);
        void deliteSpell(int index);
        void printHeand();
        void generateRandomSpell();

        void info(int sp);

        SpellCard* chooseSpell();

        std::vector<SpellCard*> getSpells();

        void cleanHeand();
        void addSpell(std::string name);
};
