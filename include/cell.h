#pragma once
#include "character.h"

enum Type
{
    SLOW,
    WALL,
    SPACE,
    TRAP
};

enum Ocupant
{
    NOOCUP = 0,
    PLAYER,
    ENEMY,
    ENEMYHUT,
    //lb2
    TOWER,
    ALLY
};

class Character;

class Cell
{
private:
    Type typeCell;
    Ocupant cellOcupant;

    //lb2
    bool readyToUse;

public:
    // Constructor
    Cell(Type type, Ocupant ocup = NOOCUP);

    // getter
    Type getType();
    Ocupant getOcupant();

    // setters
    void setType(Type type);
    void setOcupant(Ocupant ocupant);


    //spells
    void setReadyToUse(bool rtu);
    bool getReady();
};
