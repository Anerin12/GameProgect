#include "game.h"
#define FALL Position {-1,-1}


Game::Game()
{

    char opt;
    std::cout << "For start new game input - N" << std::endl << "For load game input - L" << std::endl;
    std::cin >> opt;

    switch (opt)
    {
    case 'L':
        this->loadGame();
        break;

    case 'N':
        level_ = 0;
        player_ = std::make_unique<Player>(gameConf.playerHealth, gameConf.playerDamage, gameConf.playerStartPos);
        field_ = Field(gameConf.fieldSize);
        field_.addPlayer(player_->getPosition());
        enemyManager_ = EnemyManager(field_);
        spellManager_ = SpellManager();
        break;

    default:
        level_ = 0;
        player_ = std::make_unique<Player>(gameConf.playerHealth, gameConf.playerDamage, gameConf.playerStartPos);
        field_ = Field(gameConf.fieldSize);
        field_.addPlayer(player_->getPosition());
        enemyManager_ = EnemyManager(field_);
        spellManager_ = SpellManager();
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
            saveGame();
            break;

        case 'L':
            loadGame();
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
    std::cout << "| Open Hand R | Cast trap G | Call spell X | Improve spell I |" << std::endl;
    std::cout << "| Save game M | Load game L |" << std::endl;
    std::cout << std::string(60, '_') << std::endl;
}





void Game::saveGame(){

    const auto& allies_ = spellManager_.getAlly();
    const auto& enemies_ = enemyManager_.getEnemy();
    const auto& enemyHuts_ = enemyManager_.getEnemyHut();
    const auto& towers_ = enemyManager_.getTowers();

    std::string filename;
    std::cout << "Input save name: " << std::endl;
    std::cin >> filename;

    std::ofstream out;
    out.open(std::format("../save/{}.txt", filename));
    if (out.is_open()){

        out << std::format("GameLvl {}", this->level_) << std::endl;

        std::string player = std::format("Player {} {} {} {} {} {} {}", player_->getHealth(), player_->getDamage(), player_->getPosition().x, player_->getPosition().y, player_->getWeapon(), player_->getLevel(), player_->getScore());
        out << player << std::endl;

        const auto& spells = player_->getHand()->getSpells();

        for (auto& sp : spells){
            std::string spell = std::format("Spell {}", sp->name());
            out << spell << std::endl;
        }

        auto field = field_.getField();

        for (auto y : field){
            std::string cells = "Field";
            for (auto x : y)
            {
                cells += std::format(" {} ", x.string());
            }
            out << cells << std::endl;
        }

        for (int i = 0; i < enemies_.size(); i++){
            std::string en = std::format("Enemy {} {} {} {}", enemies_[i]->getHealth(), enemies_[i]->getDamage(), enemies_[i]->getPosition().x, enemies_[i]->getPosition().y);
            out << en << std::endl;
        }

        for (int i = 0; i < enemyHuts_.size(); i++)
        {
            std::string enH = std::format("EnemyHut {} {} {}", enemyHuts_[i]->getHealth(), enemyHuts_[i]->getPosition().x, enemyHuts_[i]->getPosition().y);
            out << enH << std::endl;
        }

        for (int i = 0; i < towers_.size(); i++)
        {
            std::string tw = std::format("Tower {} {} {}", towers_[i]->getHealth(), towers_[i]->getPosition().x, towers_[i]->getPosition().y);
            out << tw << std::endl;
        }

        for (int i = 0; i < allies_.size(); i++)
        {
            std::string al = std::format("Ally {} {} {} {}", allies_[i]->getHealth(), allies_[i]->getDamage(), allies_[i]->getPosition().x, allies_[i]->getPosition().y);
            out << al << std::endl;
        }

    }
    out.close();
}


void Game::loadGame(){
    std::string folderPath = "../save";
    std::vector<std::string> saves;
    int i = 0;

    for (const auto &entry : std::filesystem::directory_iterator(folderPath))
    {
        if (entry.is_regular_file())
        {
            std::string fileName = entry.path().filename().string();
            std::string filePath = entry.path().string();
            std::cout << i++ << ". " << fileName << std::endl;
            saves.push_back(filePath);
        }
    }

    int hl, dm, lvl, sc;
    Position pos = {-1, -1};
    std::string wp;

    std::cout << "Choose save: ";
    int opt;
    std::cin >> opt;

    if (opt < 0 || opt >= saves.size())
    {
        std::cerr << "Invalid save index\n";
        return;
    }

    std::ifstream in(saves[opt]);
    if (!in.is_open())
    {
        std::cerr << "Cannot open file\n";
        return;
    }

    enemyManager_.cleanEnemies();
    spellManager_.cleanAlly();

    std::vector<std::vector<int>> loadField;

    std::vector<std::string> spells;

    std::string type;
    while (in >> type)
    {
        if (type == "Player")
        {
            in >> hl >> dm >> pos.x >> pos.y >> wp >> lvl >> sc;
            player_ = std::make_unique<Player>(hl, dm, pos);
            player_->setWeapon(wp);
            player_->setLevel(lvl);
            player_->setScore(sc);
        }

        else if (type == "Field")
        {
            std::vector<int> row;
            std::string line;
            std::getline(in, line);
            std::stringstream ss(line);

            int val;
            while (ss >> val)
                row.push_back(val);

            loadField.push_back(row);
        }

        else if (type == "Spell")
        {
            std::string spellName;
            std::getline(in, spellName);
            spellName.erase(spellName.begin());
            spells.push_back(spellName);
        }

        else if (type == "GameLvl")
        {
            in >> lvl;
            level_ = lvl;
        }

        else if (type == "Enemy") 
        {
            in >> hl >> dm >> pos.x >> pos.y;
            enemyManager_.addEnemy(std::make_unique<Enemy>(hl, dm, pos));
        }
        else if (type == "EnemyHut")
        {
            in >> hl >> pos.x >> pos.y;
            enemyManager_.addEnemyHut(std::make_unique<EnemyHut>(hl, pos));
        }
        else if (type == "Tower")
        {
            in >> hl >> pos.x >> pos.y;
            enemyManager_.addTower(std::make_unique<Tower>(hl, pos));
        }
        else if (type == "Ally")
        {
            in >> hl >> dm >> pos.x >> pos.y;
            spellManager_.addAlly(std::make_unique<Ally>(hl, dm, pos));
        }
    }

    if (loadField.empty())
    {
        std::cerr << "Field data missing in save file\n";
        return;
    }

    field_.buildNewField(loadField);

    player_->getHand()->cleanHand();

    const auto& allies_ = spellManager_.getAlly();
    const auto& enemies_ = enemyManager_.getEnemy();
    const auto& enemyHuts_ = enemyManager_.getEnemyHut();
    const auto& towers_ = enemyManager_.getTowers();

    for (auto sp : spells){
        player_->getHand()->addSpell(sp);
    }

    field_.addPlayer(player_->getPosition());

    for (auto& en : enemies_)
    {
        field_.addEnemy(en->getPosition());
    }

    for (auto& enH : enemyHuts_)
    {
        field_.addEnemy(enH->getPosition());
    }

    for (auto& al : allies_)
    {
        field_.addEnemy(al->getPosition());
    }

    for (auto& tw : towers_)
    {
        field_.addEnemy(tw->getPosition());
    }
}