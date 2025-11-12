#pragma once
#include <string>
#include "character.h"
#include "spells/trapSpell.h"
#include "spells/callSpell.h"

class Heand;
class Enemy;

class Player : public Character{
    private:
        int level_;
        int score_;
        std::string weapon_;

        //lb2
        Heand* heand_;
        std::vector<TrapSpell *> traps_;
        CallSpell *callSpell_;

    public:
        // Constructor/Destructor
        Player(int health, int damage, Position position);
        Player();

        //Active methods
        void attack(Enemy& enemy);
        void changeWeapon();
        void levelUp();
        void scoreUp();
        void printStatus();

        //getters
        int getLevel();
        int getScore();
        std::string getWeapon();

        //lb2
        Heand* getHeand();
        void useTrap();
        void deliteTrap(Position position);
        std::vector<TrapSpell*> getTraps();
        int getTrapDamage();
        void setTrapDamage(int TrapDamage);

        CallSpell* getCallSpell();
};