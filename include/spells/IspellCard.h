#pragma once
#include <string> 
#include <utility>

class ISpellCard
{
public:
    virtual ~ISpellCard() = default;
    virtual std::pair<int, int> use();
    virtual void use(ISpellCard &spell);
    virtual std::string name() const = 0;
    virtual void levelUp();
    virtual int getRadius();
};


