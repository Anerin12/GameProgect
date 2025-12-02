#include "managment/spellManager.h"
#include "managment/enemyManager.h"
#include <iostream>

#define FALL Position {-1, -1}

SpellManager::SpellManager() {};

void SpellManager::cast(ISpellCard &base, Field &field, EnemyManager &enM, Hand *hand)
{

    if (auto *as = dynamic_cast<AreaSpell *>(&base))
        return cast(*as, field, enM, hand);
    if (auto *ds = dynamic_cast<DirectSpell *>(&base))
        return cast(*ds, field, enM, hand);
    if (auto *cs = dynamic_cast<CallSpell *>(&base))
        return cast(*cs, field, enM, hand);
    if (auto *ts = dynamic_cast<TrapSpell *>(&base))
        return cast(*ts, field, enM, hand);
    if (auto *is = dynamic_cast<ImproveSpell *>(&base))
        return cast(*is, field, enM, hand);

    throw std::runtime_error("Unknown spell type");
}



void SpellManager::cast(AreaSpell &spell, Field &field, EnemyManager &enM, Hand *hand)
{
    Position center = field.chooseRegion(spell.getRadius());
    std::vector<Position> enP = field.enHInRadius(center, spell.getRadius());

    for (auto pos : enP){
        enM.setDamage(pos, spell.use().first);
    }
}

void SpellManager::cast(DirectSpell &spell, Field &field, EnemyManager &enM, Hand *hand)
{
    Position plPos = field.getPlayer();
    std::vector<Position> enemy = field.enHInRadius(plPos, spell.getRadius());
    std::cout << spell.getRadius() << std::endl;
    if (enemy.empty()){
        std::cout << "Враги не найдены" << std::endl;
        return;
    }
    int i = 1;
    for (auto en : enemy){
        std::cout << i++ << '.' << " Enemy: " << "(" << en.x << ", " << en.y << ")" << std::endl;
    }

    int num;
    std::cin >> num;

    if(num > enemy.size()){
        std::cout << "Некорректный ввод" << std::endl;
        return;
    }

    enM.setDamage(enemy[num-1], spell.use().first);
}

void SpellManager::cast(CallSpell &spell, Field &field, EnemyManager &enM, Hand *hand)
{
    Position newPos = field.getPlayer();
    newPos.y++;

    if(field.isFree(newPos)){
        allies_.push_back(std::make_unique<Ally>(100, 10, newPos));
        field.addAlly(newPos);
    }
}

void SpellManager::cast(TrapSpell &spell, Field &field, EnemyManager &enM, Hand *hand)
{
    Position newPos = field.getPlayer();
    traps_.push_back(std::make_unique<TrapSpell>(10, newPos));
    field.addTrap(newPos);
}
void SpellManager::cast(ImproveSpell &spell, Field &field, EnemyManager &enM, Hand *hand)
{
    const auto& spells = hand->getSpells();

    hand->printHand();

    int opt;
    std::cin >> opt;

    spell.use(*spells[opt-1]);
}



void SpellManager::allyMove(Field& field, EnemyManager &enM){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(-1, 1);

    if (allies_.empty()){
        return;
    }

    int index = 0;
    for (auto &al : allies_)
    {

        if (!al->isAlive())
        {
            field.deliteCharacter(al->getPosition());
            allies_.erase(allies_.begin() + index);
            continue;
        }

        Position enemyPos = field.enemyInRadius(al->getPosition(), 1);

        if (enemyPos != FALL)
        {
            enM.setDamage(enemyPos, al->getDamage());
        }
        else
        {
            Position newPos = al->getPosition();
            newPos.x += dist(gen);
            newPos.y += dist(gen);

            if (field.isFree(newPos))
            {
                field.deliteCharacter(al->getPosition());
                field.addAlly(newPos);
                al->move(newPos);
            }
        }
        index++;
    }
}

const std::vector<std::unique_ptr<Ally>>& SpellManager::getAlly() const{
    return allies_;
}

void SpellManager::addAlly(std::unique_ptr<Ally> al){
    allies_.push_back(std::move(al));
}

void SpellManager::cleanAlly(){
    allies_.clear();
}

int SpellManager::getTrapDamage(){
    return 10;
}

void SpellManager::deliteTrap(Position pos)
{
    auto it = std::remove_if(traps_.begin(), traps_.end(),
                             [&](const std::unique_ptr<TrapSpell> &t)
                             { return t->position() == pos; });
    traps_.erase(it, traps_.end());
}