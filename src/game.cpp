#include "game.h"
#define FALL Position {-1,-1}

conf_t conf{10, 100, 15, 1, 100, 10, 1, 100, 1, 100, 10};
Position startPos = {1, 1};

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
        player_ = std::make_unique<Player>(conf.playerHealth, conf.playerDamage, startPos);
        field_ = Field(conf.fieldSize);
        field_.addPlayer(player_->getPosition());
        generateEnemy(conf.numEnemy);
        generateEnemyHut(conf.numEnemyHut);
        generateTower(conf.numTower);
        break;

    default:
        level_ = 0;
        player_ = std::make_unique<Player>(conf.playerHealth, conf.playerDamage, startPos);
        field_ = Field(conf.fieldSize);
        field_.addPlayer(player_->getPosition());
        generateEnemy(conf.numEnemy);
        generateEnemyHut(conf.numEnemyHut);
        generateTower(conf.numTower);
        break;
    }

    gameLoop();
}

void Game::improvePlayer(){
    char opt;
    std::cout << "| Health H | Damage D | Spell S |" << std::endl;
    std::cin >> opt;
    switch (opt)
    {
    case 'H':
        conf.playerHealth += 20;
        player_->setHealth(conf.playerHealth);
        std::cout << "Health: " << player_->getHealth() << std::endl;
        break;

    case 'D':
        conf.playerDamage += 10;
        player_->setDamage(conf.playerDamage);
        break;

    case 'S':
        improveManager();
        break;

    default:
        break;
    }
}

void Game::improveEnemy(){
    conf.enemyDamage+=5;
    conf.enemyHutHealth+=10;
    conf.towerHealth+=10;
}

void Game::clearCharacter(){
    enemies_.clear();
    enemyHuts_.clear();
    towers_.clear();
    allies_.clear();
}

void Game::newLevel(){
    this->improvePlayer();
    this->improveEnemy();

    this->clearCharacter();

    player_->move(startPos);

    int spSize = player_->getHeand()->getSpells().size();
    for (int i = 0; i < spSize/2; i++){
        player_->getHeand()->deliteSpell(i);
    }

    field_ = (conf.fieldSize < 25)? Field(conf.fieldSize+=5) : Field(conf.fieldSize);
    field_.addPlayer(player_->getPosition());

    generateEnemy(conf.numEnemy+=2);
    generateEnemyHut(conf.numEnemyHut+=2);
    generateTower(conf.numTower+=2);
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

void Game::generateEnemy(int numEnemy)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, field_.getHeight() - 1);

    for (int i = 0; i < numEnemy; i++)
    {
        Position randPos = {dist(gen), dist(gen)};
        while(!field_.isFree(randPos)){
            randPos = {dist(gen), dist(gen)};
        }

        Enemy *en = new Enemy(conf.enemyHealth, conf.enemyDamage, randPos);
        field_.addEnemy(randPos);
        this->enemies_.push_back(en);
    }
}

void Game::generateEnemyHut(int numEnemyHut)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, field_.getHeight() - 1);

    for (int i = 0; i < numEnemyHut; i++)
    {
        Position randPos = {dist(gen), dist(gen)};
        while (!field_.isFree(randPos))
        {
            randPos = {dist(gen), dist(gen)};
        }

        EnemyHut *enH = new EnemyHut(conf.enemyHutHealth, randPos);
        field_.addEnemyHut(randPos);
        this->enemyHuts_.push_back(enH);
    }
}

void Game::generateTower(int numTower){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, field_.getHeight() - 1);

    for (int i = 0; i < numTower; i++)
    {
        Position randPos = {dist(gen), dist(gen)};
        while (!field_.isFree(randPos))
        {
            randPos = {dist(gen), dist(gen)};
        }

        Tower *tower = new Tower(conf.towerHealth, randPos);
        field_.addTower(randPos);
        this->towers_.push_back(tower);
    }
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
        Position enemyPos = {-1, -1};

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
            enemyPos = field_.enemyInRadius(player_->getPosition(), attackRadius);
            if(enemyPos != FALL){
                for(auto en : enemies_){
                    if(en->getPosition() == enemyPos){
                        player_->attack(*en);
                    }
                }
            }
            break;
        case 'E':
            player_->changeWeapon();
            break;
        case 'R':
            spellManager();
            break;

        case 'X':
            callSpellManager();
            break;

        case 'M':
            saveGame();
            break;

        case 'L':
            loadGame();
            break;

        case 'G':
            player_->useTrap();
            field_.addTrap(player_->getPosition());
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

        enemiesManager();
        enemyHutsManager();
        towerManager();

        if(!allies_.empty()){
            allyManager();
        }

        if(enemies_.empty() && enemyHuts_.empty()){
            this->newLevel();
        }

    }
}

void Game::enemiesManager(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(-1, 1);

    int index = 0;
    for (Enemy* en : enemies_){

        if(!en->isAlive()){
            field_.deliteCharacter(en->getPosition());
            enemies_.erase(enemies_.begin() + index);
            if (player_->getHealth() < conf.playerHealth){
                player_->setHealth(player_->getHealth()+10);
            }
            player_->scoreUp();
            if((player_->getScore()/10)%2 == 0){
                player_->getHeand()->generateRandomSpell();
            }
            if (player_->getScore() == 100)
            {
                player_->setDamage(conf.playerHealth);
            }
            player_->levelUp();
            std::cout << conf.playerHealth << std::endl;
            continue;
        }

        if (field_.isTrap(en->getPosition())){
            en->takeDamage(player_->getTrapDamage());
            field_.deliteTrap(en->getPosition());
            player_->deliteTrap(en->getPosition());
        }

        Position playerPos = field_.playerInZone(en->getPosition(), 1);

        if(playerPos != FALL){
            en->attack(*player_);
        }
        else{
            Position newPos = en->getPosition();
            newPos.x+=dist(gen);
            newPos.y+=dist(gen);

            if(field_.isFree(newPos)){
                field_.deliteCharacter(en->getPosition());
                field_.addEnemy(newPos);
                en->move(newPos);
            }
        }
        index++;
    }
}

void Game::enemyHutsManager(){
    int index = 0;

    for(auto enH : enemyHuts_){
        if(!enH->isAlive()){
            field_.deliteCharacter(enH->getPosition());
            enemyHuts_.erase(enemyHuts_.begin() + index);
        }

        if(enH->update()){
            enemies_.push_back(enH->generateEnemy());
        }
        index++;
    }
}


void Game::printSettings() {
    std::cout << "GameLevel: " << level_ << std::endl;
    std::cout << std::string(60, '_') << std::endl;
    std::cout << "| Move: W/S/A/D | Change weapon: E | Attack: F | Quit: Q |" << std::endl;
    std::cout << "| Open Heand R | Cast trap G | Call spell X | Improve spell I |" << std::endl;
    std::cout << "| Save game M | Load game L |" << std::endl;
    std::cout << std::string(60, '_') << std::endl;
}

void Game::spellManager(){
    player_->getHeand()->printHeand();
    SpellCard *spell = player_->getHeand()->chooseSpell();
    if(spell){
        auto spellInf = spell->use(field_, player_->getPosition());
        std::vector<Position> inRad = spellInf.first;
        int damage = spellInf.second;

        for (Position pos : inRad){
            for (Enemy* en : enemies_){
                if (pos == en->getPosition()){
                    en->takeDamage(damage);
                }
            }

            for (EnemyHut *enH : enemyHuts_){
                if (pos == enH->getPosition()){
                    enH->takeDamage(damage);
                }
            }
        }
    }

}


void Game::towerManager(){

    for (auto tw : towers_){
        if(field_.playerInZone(tw->getPosition(), tw->attack()->getRadius()) != FALL){
            auto damage = tw->attack()->getDamage();
            player_->takeDamage(damage);

        }
    }
}


void Game::callSpellManager(){
    CallSpell *newSpell = player_->getCallSpell();
    auto spell = newSpell->use(field_, player_->getPosition());
    Position allyPos = spell.first[0];

   for (int num = 0; num < newSpell->getNumAlly(); num++){
        Ally *al = new Ally(100, 20, allyPos);
        allies_.push_back(al);
    }
}

void Game::allyManager(){

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(-1, 1);

    int index = 0;
    for (Ally *al : allies_)
    {

        if (!al->isAlive())
        {
            field_.deliteCharacter(al->getPosition());
            allies_.erase(allies_.begin() + index);
            continue;
        }

        Position enemyPos = field_.enemyInRadius(al->getPosition(), 1);

        if (enemyPos != FALL)
        {
            for (auto en : enemies_){
                if (en->getPosition() == enemyPos){
                    al->attack(*en);
                }
            }
        }
        else
        {
            Position newPos = al->getPosition();
            newPos.x += dist(gen);
            newPos.y += dist(gen);

            if (field_.isFree(newPos))
            {
                field_.deliteCharacter(al->getPosition());
                field_.addAlly(newPos);
                al->move(newPos);
            }
        }
        index++;
    }
}

void Game::improveManager(){
    ImproveSpell improve;
    char opt;
    int n;
    std::cout << "| Call spell Z | Trap spell X | Spell in heand C |" << std::endl;
    const std::vector<std::unique_ptr<TrapSpell>> &traps = player_->getTraps();
    std::vector<SpellCard*> spells;
    std::cin >>opt;

    switch (opt)
    {
    case 'Z':
        std::cout << "Улучшаю заклинание призыва..." << std::endl;
        improve.use(player_->getCallSpell());
        break;
    case 'X':
        std::cout << "Улучшаю заклинание ловушки..." << std::endl;
        for (auto& trap : traps){
            improve.use(trap.get());
        }
        player_->setTrapDamage(player_->getTrapDamage() + 10);
        break;

    case 'C':
        spells = player_->getHeand()->getSpells();
        player_->getHeand()->printHeand();
        std::cin >> n;
        std::cout << "Улучшаю заклинание " << spells[n-1]->getname() << "... " << std::endl;
        improve.use(spells[n-1]);

    default:
        break;
    }

}


void Game::saveGame(){

    std::string filename;
    std::cout << "Input save name: " << std::endl;
    std::cin >> filename;

    std::ofstream out;
    out.open(std::format("../save/{}.txt", filename));
    if (out.is_open()){

        out << std::format("GameLvl {}", this->level_) << std::endl;

        std::string player = std::format("Player {} {} {} {} {} {} {}", player_->getHealth(), player_->getDamage(), player_->getPosition().x, player_->getPosition().y, player_->getWeapon(), player_->getLevel(), player_->getScore());
        out << player << std::endl;

        auto spells = player_->getHeand()->getSpells();

        for (auto sp : spells){
            std::string spell = std::format("Spell {}", sp->getname());
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

    enemies_.clear();
    towers_.clear();
    allies_.clear();
    enemyHuts_.clear();

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
            enemies_.push_back(new Enemy(hl, dm, pos));
        }
        else if (type == "EnemyHut")
        {
            in >> hl >> pos.x >> pos.y;
            enemyHuts_.push_back(new EnemyHut(hl, pos));
        }
        else if (type == "Tower")
        {
            in >> hl >> pos.x >> pos.y;
            towers_.push_back(new Tower(hl, pos));
        }
        else if (type == "Ally")
        {
            in >> hl >> dm >> pos.x >> pos.y;
            allies_.push_back(new Ally(hl, dm, pos));
        }
    }

    if (loadField.empty())
    {
        std::cerr << "Field data missing in save file\n";
        return;
    }

    field_.buildNewField(loadField);

    player_->getHeand()->cleanHeand();

    for (auto sp : spells){
        player_->getHeand()->addSpell(sp);
    }

    field_.addPlayer(player_->getPosition());

    for (auto en : enemies_)
    {
        field_.addEnemy(en->getPosition());
    }

    for (auto enH : enemyHuts_)
    {
        field_.addEnemy(enH->getPosition());
    }

    for (auto al : allies_)
    {
        field_.addEnemy(al->getPosition());
    }

    for (auto tw : towers_)
    {
        field_.addEnemy(tw->getPosition());
    }
}