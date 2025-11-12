#include "spells/directSpell.h"

DirectSpell::DirectSpell(std::string name, std::string description, int damage, int radius):
    SpellCard(name, description, damage, radius){}

// Active methods

std::pair<std::vector<Position>, int> DirectSpell::use(Field &field, Position playerPos)
{
    std::cout << "Выберите врага: " << std::endl;

    auto &cells = field.getField();
    int height = field.getHeight();
    std::vector<std::pair<Ocupant, Position>> enemies;

    for (int y = playerPos.y - this->getRadius(); y <= playerPos.y + this->getRadius(); y++)
    {
        for (int x = playerPos.x - this->getRadius(); x <= playerPos.x + this->getRadius(); x++)
        {
            if (y < 0 || x < 0 || y >= height || x >= height)
                continue;
            if (cells[y][x].getOcupant() == ENEMY || cells[y][x].getOcupant() == ENEMYHUT)
            {
                Position pos = {x,y};
                enemies.emplace_back(cells[y][x].getOcupant(), pos);
            }
        }
    }

    if (enemies.empty())
    {
        std::cout << "В радиусе нет врагов!" << std::endl;
        return {{{-1, -1}}, -1};
    }

    int i = 1;
    for (auto &en : enemies)
    {
        std::cout << i++ << ". "
                  << ((en.first == ENEMY) ? "Enemy" : "Enemy Hut")
                  << " (" << en.second.x << ", "
                  << en.second.y << ")\n";
    }

    int command;
    std::cin >> command;
    

    if (command < 1 || command > static_cast<int>(enemies.size()))
    {
        if (command == 0){
            std::cout << "Отмена заклинания..." << std::endl;
            return {{{-1, -1}}, -1};
        }
        else
        {
            std::cout << "Неверный выбор!" << std::endl;
            return {{{-1, -1}}, -1};
        }
    }

    std::vector<Position> chooseEnemy = {enemies[command-1].second};

    std::pair<std::vector<Position>, int> result = {chooseEnemy, this->getDamage()};

    return result;
}
