#include "managment/game.h"
#define FALL Position {-1,-1}


Game::Game()
{

    char opt;
    std::cout << "For start new game input - N" << std::endl << "For load game input - L" << std::endl;
    std::cin >> opt;

    level_ = 0;
    player_ = std::make_unique<Player>(gameConf.playerHealth, gameConf.playerDamage, gameConf.playerStartPos);
    field_ = Field(gameConf.fieldSize);
    field_.addPlayer(player_->getPosition());
    enemyManager_ = EnemyManager(field_);
    spellManager_ = SpellManager();

    switch (opt)
    {
    case 'L':
        saveManager_.load(spellManager_, enemyManager_, field_, *player_, level_);
        break;
    default:
        break;
    }

    gameLoop();
}

void Game::improvePlayer(){
    char opt;
    std::cout << "| Health H | Damage D | Spell S |" << std::endl;
    std::cin >> opt;
    std::unique_ptr<ImproveSpell> spell;
    switch (opt)
    {
    case 'H':
        gameConf.playerHealth += 20;
        player_->setHealth(gameConf.playerHealth);
        std::cout << "Health: " << player_->getHealth() << std::endl;
        break;

    case 'D':
        gameConf.playerDamage += 10;
        player_->setDamage(gameConf.playerDamage);
        break;

    case 'S':
        spell = std::make_unique<ImproveSpell>();
        spellManager_.cast(*spell, field_, enemyManager_, player_->getHand());
        break;

    default:
        break;
    }
}


void Game::newLevel(){
    this->improvePlayer();
    enemyManager_.improveEnemies();

    enemyManager_.cleanEnemies();
    spellManager_.cleanAlly();

    player_->move(gameConf.playerStartPos);

    int spSize = player_->getHand()->size();
    for (int i = 0; i < spSize/2; i++){
        player_->getHand()->deliteSpell(i);
    }

    field_ = (gameConf.fieldSize < 25)? Field(gameConf.fieldSize+=5) : Field(gameConf.fieldSize);
    field_.addPlayer(player_->getPosition());
    enemyManager_.generateEnemies(field_);

    level_++;
}


void Game::gameOver()
{
    isRuning_ = false;
    std::cout << std::string(50, '_') << std::endl;
    std::cout << std::string(20, '_') << "GAME OVER" << std::string(20, '_') << std::endl;
    std::cout << std::string(20, '_') << "SCORE: " << (player_->getLevel()-1)*100 + player_->getScore() << std::string(20, '_') << std::endl;
    std::cout << std::string(50, '_') << std::endl;
}


void Game::gameLoop()
{
    while (isRuning_)
    {
        if (!player_->isAlive())
        {
            gameOver();
            break;
        }

        player_->printStatus();
        field_.print();
        printSettings();

        char opts;
        std::cout << "Input command: ";
        std::cin >> opts;

        int attackRadius = (player_->getWeapon() == "knight")? 1 : 4;
        Position enemyPos = {-1,-1};
        std::unique_ptr<ISpellCard> spell;
        Position newPos = player_->getPosition();

        switch (opts)
        {
        case 'W':
            newPos.y--;
            break;
        case 'S':
            newPos.y++;
            break;
        case 'A':
            newPos.x--;
            break;
        case 'D':
            newPos.x++;
            break;
        case 'Q':
            gameOver();
            break;
        case 'F':
        {
            enemyPos = field_.enemyInRadius(player_->getPosition(), attackRadius);
            enemyManager_.setDamage(enemyPos, player_->getDamage());
            break;
        }
        case 'E':
            player_->changeWeapon();
            break;
        case 'R':
            player_->getHand()->printHand();
            spell = player_->getHand()->chooseSpell();
            if (spell != nullptr)
                spellManager_.cast(*spell, field_, enemyManager_, player_->getHand());

            break;

        case 'M':
            saveManager_.save(spellManager_, enemyManager_, field_, *player_, level_);
            break;

        case 'L':
            saveManager_.load(spellManager_, enemyManager_, field_, *player_, level_); 
            break;

        default:
            std::cout << "Неверная комманда\n";
            break;
        }


        if(!player_->thisIsSlow()){    
            if(field_.isFree(newPos)){
                field_.deliteCharacter(player_->getPosition());
                player_->move(newPos);
                field_.addPlayer(newPos);
                if (field_.isSlow(newPos)) player_->setIsSlow(true);
            }
        }
        else{
            player_->setIsSlow(!player_->thisIsSlow());
        }

        if(enemyManager_.enemyEmpty()){
            this->newLevel();
        }

        enemyManager_.enemyMove(*player_, field_, spellManager_);
        enemyManager_.enemyHutsMove(*player_, field_);
        enemyManager_.towerMove(*player_, field_);
        spellManager_.allyMove(field_, enemyManager_);
    }
}



void Game::printSettings() {
    std::cout << "GameLevel: " << level_ << std::endl;
    std::cout << std::string(60, '_') << std::endl;
    std::cout << "| Move: W/S/A/D | Change weapon: E | Attack: F | Quit: Q |" << std::endl;
    std::cout << "| Open Hand R |" << std::endl;
    std::cout << "| Save game M | Load game L |" << std::endl;
    std::cout << std::string(60, '_') << std::endl;
}


