#include "spells/spellCard.h"

SpellCard::SpellCard(std::string name, std::string description, int damage, int radius): 
            name_(name), description_(description), damage_(damage), radius_(radius){}


// Active methods
std::pair<std::vector<Position>, int> SpellCard::use(Field &field, Position playerPos) { return {{{-1, -1}}, -1} ;}

// setters
void SpellCard::setDamage(int damage){this->damage_ = damage;}

// getters
std::string SpellCard::getname(){return name_;}
std::string SpellCard::getDescription(){return description_;}
int SpellCard::getDamage(){return damage_;}

void SpellCard::getInfo(){
    std::cout << "Информация: " << std::endl;
    std::cout << name_ << std::endl;
    std::cout << "Описание: " << description_ << std::endl;
    std::cout <<"Урон: " << damage_ <<std::endl;
}

int SpellCard::getRadius(){return radius_;}
void SpellCard::setRadius(int radius){radius_ = radius;}
