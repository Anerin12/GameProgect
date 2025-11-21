#include "heand.h"
#include "spells/areaSpell.h"
#include "spells/directSpell.h"

std::vector<std::string> directNames = {"Огненный шар", "Малый разряд", "Большой разряд"};
std::vector<std::string> directDescriptions = {"Поражает врага адским пламенем", "Поражает врага коротким зарядом", "Разряд, от которого жарится плоть"};
std::vector<int> directDamage = {50, 30, 100};
std::vector<int> directRadius = {5, 7, 4};

std::vector<std::string> areaNames = {"Гололед", "Иссушающее пламя", "Чума"};
std::vector<std::string> areaDescriptions = {"Попробуй устоять на ногах", "Никто не выживет", "Пора надеть маску"};
std::vector<int> areaDamage = {30, 100, 70};
std::vector<int> areaRadius = {7, 4, 9};

Heand::Heand(int size) : size_(size)
{
    generateRandomSpell();
}

Heand::~Heand()
{
    for (auto spell : spells_)
    {
        delete spell;
    }
    spells_.clear();
}

void Heand::addSpell(SpellCard *spell)
{
    spells_.push_back(spell);
}

void Heand::deliteSpell(int index)
{
    if (index >= 0 && index < static_cast<int>(spells_.size()))
    {
        spells_.erase(spells_.begin() + index);
    }
}

void Heand::printHeand()
{
    if (spells_.empty())
    {
        std::cout << "Заклинаний нет!" << std::endl;
        return;
    }

    int i = 1;
    std::cout << "Заклинания: " << std::endl;
    std::cout << "0. Отмена " << std::endl;

    for (SpellCard *spell : spells_)
    {
        std::cout << i++ << ". " << spell->getname() << std::endl;
    }

}

void Heand::generateRandomSpell()
{
    if (size_ <= spells_.size()){return;}

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 1);

    std::random_device rand;
    std::mt19937 genem(rand());
    std::uniform_int_distribution<> distruct(0, 2);

    int type = dist(gen);

    if (type)
    {
        int directSpell = distruct(genem);
        spells_.push_back(new DirectSpell(directNames[directSpell], directDescriptions[directSpell], directDamage[directSpell], directRadius[directSpell]));
    }
    else
    {
        int areaSpell = distruct(genem);
        spells_.push_back(new AreaSpell(areaNames[areaSpell], areaDescriptions[areaSpell], areaDamage[areaSpell], areaRadius[areaSpell]));
    }
}

SpellCard *Heand::chooseSpell()
{
    std::cout << "Выбери заклинание: ";
    int sp;
    std::cin >> sp;

    if (sp < 1 || sp > static_cast<int>(spells_.size()))
    {
        if (sp == 0)
        {
            return nullptr;
        }
        std::cout << "Неверный выбор!" << std::endl;
        return nullptr;
    }

    SpellCard *spell = spells_[sp - 1];
    this->info(sp);
    deliteSpell(sp - 1);

    return spell;
}

void Heand::info(int sp){
    char opt;
    std::cout << "Посмотреть информацию Y/N" <<std::endl;

    std::cin >> opt;

    switch (opt)
    {
    case 'Y':
        spells_[sp-1]->getInfo();
        break;

    case 'N':
        break;

    default:
        break;
    }
}

std::vector<SpellCard*> Heand::getSpells(){return spells_;}

void Heand::cleanHeand(){
    spells_.clear();
}

void Heand::addSpell(std::string name){
    for (int sp = 0; sp < areaNames.size(); sp++){
        if (areaNames[sp] == name){
            SpellCard* spell = new SpellCard(name, areaDescriptions[sp], areaDamage[sp], areaRadius[sp]);
            spells_.push_back(spell);
            return;
        }
    }

    for (int sp = 0; sp < directNames.size(); sp++)
    {
        if (directNames[sp] == name)
        {
            SpellCard *spell = new SpellCard(name, directDescriptions[sp], directDamage[sp], directRadius[sp]);
            spells_.push_back(spell);
            return;
        }
    }
}
