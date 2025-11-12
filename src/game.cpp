#include "game.h"
#define FALL Position {-1,-1}

Game::Game()
{
    Position startPos = {1, 1};
    player_ = Player(100, 15, startPos);
    field_ = Field(25);
    field_.addPlayer(player_.getPosition());
    generateEnemy(3);
    generateEnemyHut(3);
    generateTower(3);

    gameLoop();
}


void Game::gameOver()
{
    isRuning_ = false;
    std::cout << std::string(50, '_') << std::endl;
    std::cout << std::string(20, '_') << "GAME OVER" << std::string(20, '_') << std::endl;
    std::cout << std::string(20, '_') << "SCORE: " << (player_.getLevel()-1)*100 + player_.getScore() << std::string(20, '_') << std::endl;
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

        Enemy *en = new Enemy(100, 10, randPos);
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

        EnemyHut *enH = new EnemyHut(100, randPos);
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

        Tower *tower = new Tower(100, randPos);
        field_.addTower(randPos);
        this->towers_.push_back(tower);
    }
}

void Game::gameLoop()
{
    while (isRuning_)
    {
        if (!player_.isAlive())
        {
            gameOver();
            break;
        }

        player_.printStatus();
        field_.print();
        printSettings();

        char opts;
        std::cout << "Input command: ";
        std::cin >> opts;

        int attackRadius = (player_.getWeapon() == "knight")? 1 : 4;
        Position enemyPos = {-1, -1};

        Position newPos = player_.getPosition();

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
            enemyPos = field_.enemyInRadius(player_.getPosition(), attackRadius);
            if(enemyPos != FALL){
                for(auto en : enemies_){
                    if(en->getPosition() == enemyPos){
                        player_.attack(*en);
                    }
                }
            }
            break;
        case 'E':
            player_.changeWeapon();
            break;
        case 'R':
            spellManager();
            break;

        case 'X':
            callSpellManager();
            break;

        case 'G':
            player_.useTrap();
            field_.addTrap(player_.getPosition());
            break;

        case 'I':
            improveManager();
            break;

        default:
            std::cout << "Неверная комманда\n";
            break;
        }


        if(!player_.thisIsSlow()){    
            if(field_.isFree(newPos)){
                field_.deliteCharacter(player_.getPosition());
                player_.move(newPos);
                field_.addPlayer(newPos);
                if (field_.isSlow(newPos)) player_.setIsSlow(true);
            }
        }
        else{
            player_.setIsSlow(!player_.thisIsSlow());
        }

        enemiesManager();
        enemyHutsManager();
        towerManager();

        if(!allies_.empty()){
            allyManager();
        }

        if(enemies_.empty() && enemyHuts_.empty()){
            std::cout << "You WIN" << std::endl
;            this->gameOver();
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
            player_.scoreUp();
            if((player_.getScore()/10)%2 == 0){
                player_.getHeand()->generateRandomSpell();
            }
            player_.levelUp();
            continue;
        }

        if (field_.isTrap(en->getPosition())){
            en->takeDamage(player_.getTrapDamage());
            field_.deliteTrap(en->getPosition());
            player_.deliteTrap(en->getPosition());
        }

        Position playerPos = field_.playerInZone(en->getPosition(), 1);

        if(playerPos != FALL){
            en->attack(player_);
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
    std::cout << "| Move: W/S/A/D | Change weapon: E | Attack: F | Quit: Q |" << std::endl;
    std::cout << "| Open Heand R | Cast trap G | Call spell X | Improve spell I |" << std::endl;
    std::cout << std::string(50, '_') << std::endl;
}

void Game::spellManager(){
    SpellCard* spell = player_.getHeand()->printHeand();
    if(spell){
        auto spellInf = spell->use(field_, player_.getPosition());
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
            player_.takeDamage(damage);

        }
    }
}


void Game::callSpellManager(){
    CallSpell *newSpell = player_.getCallSpell();
    auto spell = newSpell->use(field_, player_.getPosition());
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
    std::vector<TrapSpell*> traps;
    std::vector<SpellCard*> spells;
    std::cin >>opt;

    switch (opt)
    {
    case 'Z':
        std::cout << "Улучшаю заклинание призыва..." << std::endl;
        improve.use(player_.getCallSpell());
        break;
    case 'X':
        std::cout << "Улучшаю заклинание ловушки..." << std::endl;
        traps = player_.getTraps();
        for (auto trap : traps){
            improve.use(trap);
        }
        player_.setTrapDamage(player_.getTrapDamage() + 10);
        break;

    case 'C':
        spells = player_.getHeand()->getSpells();
        std::cin >> n;
        std::cout << "Улучшаю заклинание " << spells[n-1]->getname() << "... " << std::endl;
        improve.use(spells[n-1]);

    default:
        break;
    }

}