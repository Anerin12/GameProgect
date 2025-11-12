#include "spellCard.h"

class CallSpell : public SpellCard{
    private:
        int numAlly_;
    public: 
        CallSpell(int numAlly);

        std::pair <std::vector<Position>, int> use(Field &field, Position playerPos) override;

        int getNumAlly();
        void setNumAlly(int numAlly);
};