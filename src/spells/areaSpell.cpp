#include "spells/areaSpell.h"

AreaSpell::AreaSpell(std::string name, std::string description, int damage, int radius):
    SpellCard(name, description, damage, radius){}

// Active methods
std::pair<std::vector<Position>, int> AreaSpell::use(Field &field, Position playerPos)
{


    auto cells = field.getField();

    std::cout << "Выберете область: " << std::endl;

    bool flag = true;

    std::vector<Position> enemyPos;

    while (flag)
    {
        int height = field.getHeight();
        auto &cells = field.getField(); 

        for (int y = playerPos.y - this->getRadius(); y <= playerPos.y + this->getRadius(); y++)
        {
            for (int x = playerPos.x - this->getRadius(); x <= playerPos.x + this->getRadius(); x++)
            {
                if (y < 0 || x < 0 || y >= height || x >= height)
                    continue;
                cells[y][x].setReadyToUse(true);
            }
        }

        field.print();

        char command;
        std::cin >> command;
        std::cout << "| Move: W/S/A/D | Attack: F | Quit: Q |" << std::endl;

        switch (command)
        {
        case 'W':
            playerPos.y -= 1;
            break;
        case 'S':
            playerPos.y += 1;
            break;
        case 'A':
            playerPos.x -= 1;
            break;
        case 'D':
            playerPos.x += 1;
            break;
        case 'F':
            for (int y = playerPos.y - this->getRadius(); y <= playerPos.y + this->getRadius(); y++)
            {
                for (int x = playerPos.x - this->getRadius(); x <= playerPos.x + this->getRadius(); x++)
                {
                    if (y < 0 || x < 0 || y >= height || x >= height)
                        continue; 
                    if (cells[y][x].getOcupant() == ENEMY || cells[y][x].getOcupant() == ENEMYHUT)
                    {
                        enemyPos.push_back({x,y});
                    }
                }
            }
            flag = false;
            break;
        case 'Q':
            break;
            
        default:
            break;
        }

        this->setZeroUsed(field);
    }

    this->setZeroUsed(field);

    std::pair<std::vector<Position>, int> result = {enemyPos, this->getDamage()};

    return result;
}

void AreaSpell::setZeroUsed(Field &field){
    auto &cells = field.getField();

    for (auto &y : cells)
    {
        for (auto &x : y)
        {
            x.setReadyToUse(false);
        }
    }
}