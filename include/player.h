#pragma once
#include <memory>      
#include <string>      
#include "character.h" 
#include "hand.h"      
class Enemy;           
struct Position;

class Player : public Character{
    private:
        int level_;
        int score_;
        std::string weapon_;

        //lb2
        std::unique_ptr<Hand> hand_;

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
        Hand* getHand();
        void useTrap();
        void deliteTrap(Position position);

        void setWeapon(std::string weapon);
        void setLevel(int lvl);
        void setScore(int score);
};