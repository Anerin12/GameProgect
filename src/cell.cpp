#include "cell.h"
#include "character.h"

// Constructor
Cell::Cell(Type type, Ocupant ocup)
    : typeCell(type), cellOcupant(ocup), readyToUse(false) {}

// getter
Type Cell::getType() { return typeCell; }
Ocupant Cell::getOcupant() { return cellOcupant; }

// setters
void Cell::setType(Type type) { typeCell = type; }
void Cell::setOcupant(Ocupant ocupant) { cellOcupant = ocupant; }

//spells
void Cell::setReadyToUse(bool rtu){this->readyToUse = rtu;}
bool Cell::getReady(){return readyToUse;}

