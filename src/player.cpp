#include "player.h"
#include "heand.h"
#include "enemy.h"

#define STARTPOS Position {1, 1}


// Constructor/Destructor
Player::Player(int health, int damage, Position position):Character(health, damage, position), level_(1), score_(0), weapon_("knight"), trapDamage_(100){
    heand_ = std::make_unique<Heand>(10);
    callSpell_ = std::make_unique<CallSpell>(1);
}

Player::Player() : Character(100, 20, STARTPOS), level_(1), score_(0), weapon_("knight"), trapDamage_(100)
{
    heand_ = std::make_unique<Heand>(10);
    callSpell_ = std::make_unique<CallSpell>(1);
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
        this->score_ = 0;
    }
    
}

void Player::scoreUp(){
    this->score_+=10;
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
    traps_.push_back(std::make_unique<TrapSpell>(trapDamage_, this->getPosition()));
}

void Player::deliteTrap(Position position){
    int index = 0;

    auto it = std::find_if(traps_.begin(), traps_.end(), [&](auto& trap){return trap->getPosition() == position;});

    if (it != traps_.end())
    {
        traps_.erase(it);
    }
}

void Player::setTrapDamage(int TrapDamage){
    trapDamage_ = TrapDamage;
}

void Player::setWeapon(std::string weapon){this->weapon_ = weapon;}
void Player::setLevel(int lvl){this->level_ = lvl;}
void Player::setScore(int score){this->score_ = score;}

// getters
int Player::getLevel() const {return level_;}
int Player::getScore() const {return score_;}
std::string Player::getWeapon() const {return weapon_;}
Heand* Player::getHeand(){return heand_.get();}
CallSpell* Player::getCallSpell(){return callSpell_.get();}
int Player::getTrapDamage(){return trapDamage_;}
const std::vector<std::unique_ptr<TrapSpell>>& Player::getTraps() const {return traps_;}