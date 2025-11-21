#pragma once
#include <string>
#include <memory>
#include <algorithm>
#include "character.h"
#include "heand.h"
#include "spells/trapSpell.h"
#include "spells/callSpell.h"

class Enemy;

class Player : public Character{
    private:
        int level_;
        int score_;
        std::string weapon_;
        int trapDamage_;

        //lb2
        std::unique_ptr<Heand> heand_;
        std::vector<std::unique_ptr<TrapSpell>> traps_;
        std::unique_ptr<CallSpell> callSpell_;

    public:
        // Constructor/Destructor
        Player(int health, int damage, Position position);
        Player();
        Player(Player &&) noexcept = default;
        Player &operator=(Player &&) noexcept = default;

        //Active methods
        void attack(Enemy& enemy);
        void changeWeapon();
        void levelUp();
        void scoreUp();
        void printStatus();

        //getters
        int getLevel() const;
        int getScore() const;
        std::string getWeapon() const;

        //lb2
        Heand* getHeand();
        void useTrap();
        void deliteTrap(Position position);
        const std::vector<std::unique_ptr<TrapSpell>>& getTraps() const;
        int getTrapDamage();
        void setTrapDamage(int TrapDamage);

        CallSpell* getCallSpell();


        void setWeapon(std::string weapon);
        void setLevel(int lvl);
        void setScore(int score);
};