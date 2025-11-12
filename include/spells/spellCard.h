#pragma once
#include "field.h"

class SpellCard
{
private:
    std::string name_;
    std::string description_;
    int damage_;
    int radius_;

public:
    SpellCard(std::string name, std::string description, int damage, int radius);

    //Active methods
    virtual std::pair<std::vector<Position>, int> use(Field &field, Position playerPos);

    //setters
;
    void setDamage(int damage);

    //getters
    std::string getname();
    std::string getDescription();
    int getDamage();
    void getInfo();
    int getRadius();
    void setRadius(int radius);
};


