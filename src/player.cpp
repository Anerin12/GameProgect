#include "player.h"
#include "heand.h"
#include "enemy.h"

#define STARTPOS Position {1, 1}

int trapDamage = 100;

// Constructor/Destructor
Player::Player(int health, int damage, Position position):Character(health, damage, position), level_(1), score_(0), weapon_("knight"){
    heand_ = new Heand(10);
    callSpell_ = new CallSpell(1);
}

Player::Player(): Character(100, 20, STARTPOS){
    heand_ = new Heand(10);
    callSpell_ = new CallSpell(1);
}

// Active methods
void Player::attack(Enemy& enemy)
{
    enemy.takeDamage(this->getDamage());
}

void Player::changeWeapon(){
    this->weapon_ = (weapon_ == "knight") ? "gun" : "knight";
    this->setDamage((weapon_ == "knight")? this->getDamage() : 2*this->getDamage());
}

void Player::levelUp(){
    if (this->score_ == 100){
        this->level_ += 1;
        this->setHealth(100);
        this->setDamage(this->getDamage()+10);
        this->score_ = 0;
    }
    
}

void Player::scoreUp(){
    this->score_+=10;
    this->setHealth(this->getHealth() + 20);
}

void Player::printStatus(){
    std::cout << std::string(50, '_') << std::endl;
    std::cout << "Level: " << level_ << std::endl;
    std::cout << "Score: " << score_ << std::endl;
    std::cout << "Health: " << this->getHealth() << std::endl;
    std::cout << "Weapon: " << weapon_ << std::endl;
    std::cout << std::string(50, '_') << std::endl;
}


void Player::useTrap(){
    traps_.push_back(new TrapSpell(trapDamage, this->getPosition()));
}

void Player::deliteTrap(Position position){
    int index = 0;

    for (TrapSpell *trap : traps_){
        if (trap->getPosition() == position)
            break;
        else index++;
    }

    traps_.erase(traps_.begin() + index);
}

void Player::setTrapDamage(int TrapDamage){
    trapDamage = TrapDamage;
}

// getters
int Player::getLevel(){return level_;}
int Player::getScore(){return score_;}
std::string Player::getWeapon(){return weapon_;}
Heand* Player::getHeand(){return heand_;}
CallSpell* Player::getCallSpell(){return callSpell_;}
int Player::getTrapDamage(){return trapDamage;}
std::vector<TrapSpell*> Player::getTraps(){return traps_;}