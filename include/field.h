#pragma once
#include "cell.h"
#include <vector>

class Field
{
private:
    std::vector<std::vector<Cell>> field_;
    int height_;

public:
    Field(int height);
    Field();
    Field& copy(const Field &other);
    Field(const Field &other);
    Field &operator=(const Field &other);
    Field& move(Field &&other);
    Field(Field &&other) noexcept;
    Field &operator=(Field &&other) noexcept;

    void generateField();

    void addPlayer(Position position);
    void addEnemy(Position position);
    void addEnemyHut(Position position);
    void deliteCharacter(Position position);

    
    void print();

    //chaking
    bool isFree(Position position);
    bool isSlow(Position position);
    Position playerInZone(Position position, int radius);
    Position enemyInRadius(Position position, int radius);

    std::vector<std::vector<Cell>> &getField();
    int getHeight();

    // lb2
    void addTrap(Position position);
    void deliteTrap(Position position);
    bool isTrap(Position position);
    void addTower(Position position);
    void addAlly(Position position);
    };
