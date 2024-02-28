
#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include "Level.h"
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath), bonusPts(1000)
{
    m_avatar = nullptr;
    m_actors.clear();
    m_numCrystals = 0;
}

StudentWorld::~StudentWorld(){
    cleanUp();
}

int StudentWorld::init()
{
    Level lev(assetPath());
    //build file name
    ostringstream oss;
    oss << setw(2) << setfill('0') << getLevel();
    string lvlFile = "level" + oss.str() + ".txt";
    Level::LoadResult result = lev.loadLevel(lvlFile);
    //check for file
    if (result == Level::load_fail_file_not_found)
        return GWSTATUS_PLAYER_WON;
    else if (result == Level::load_fail_bad_format)
        return GWSTATUS_LEVEL_ERROR;
    else if (result == Level::load_success)
    {
        cerr << "Successfully loaded level\n";
        //read level file
        for(int x = 0; x < 16; x++){
            for(int y = 0; y < 16; y++){
                Level::MazeEntry ge = lev.getContentsOf(x,y);
                switch (ge)
                {
                        
                        //CHECK FOR EXIT AT THE VERY BEGINNING OF THE LIST AND THEN RETRIEVE EXIT BY RETRIEVING FIRST LIST ITEM
                    
                    case Level::player:
                        m_avatar = new Avatar(this, x, y);
                        break;
                    case Level::wall:
                        m_actors.push_back(new Wall(this, x, y));
                        break;
                    case Level::pit:
                        m_actors.push_back(new Pit(this, x, y));
                        break;
                    
                    case Level::marble:
                        m_actors.push_front(new Marble(this, x, y));
                        break;
                    case Level::horiz_ragebot:
                        m_actors.push_back(new RageBot(this, x, y, Alive::right));
                        break;
                    case Level::vert_ragebot:
                        m_actors.push_back(new RageBot(this, x, y, Alive::down));
                        break;
                    case Level::thiefbot_factory:
                        m_actors.push_back(new ThiefBot(this, x, y, Actor::right));
                        break;
                    case Level::crystal:
                        m_actors.push_front(new Crystal(this, x, y));
                        m_numCrystals++;
                        break;
                    case Level::extra_life:
                        m_actors.push_front(new ExtraLifeGoodie(this, x, y));
                        break;
                    case Level::restore_health:
                        m_actors.push_front(new RestoreHealthGoodie(this, x, y));
                        break;
                    case Level::ammo:
                        m_actors.push_front(new AmmoGoodie(this, x, y));
                        break;
                               
                }
            }
        }
        
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    

    list<Actor*>::iterator it;
    for(it = m_actors.begin(); it != m_actors.end(); it++){
        if((*it)->isActive())
        {
            (*it) -> doSomething();
            if(!m_avatar->isActive())
                return GWSTATUS_PLAYER_DIED;
            //TODO: IMPLEMENT IF PLAYER COMPLETES LEVEL
        }
    }
    m_avatar -> doSomething();

    removeDeadGameObjects();
    
    if(bonusPts > 0) bonusPts--;
    
    //TODO: EXPOSE EXIT ONCE EVERYTHING COMPLETED
    
    //TODO: UPDATE STATUS
    setDisplayText();


	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    delete m_avatar;
    m_avatar = nullptr;
    list<Actor*>::iterator it;
    for(it = m_actors.begin(); it != m_actors.end(); it++){
        delete (*it);
    }
    m_actors.clear();
}

//----------------------------------------------------------HELPER FUNCTIONS----------------------------------------------------------


//ACTOR AT X,Y AND BLOCKS MOVEMENT
bool StudentWorld::actorIsBlockingAtXY(double x, double y)
{
     list<Actor*>::iterator it;
    if(m_avatar->getX() == x && m_avatar->getY() == y) 
        return true;
    for(it = m_actors.begin(); it != m_actors.end(); it++)
    {
        if((*it)->getX() == x && (*it)->getY() == y && (*it)->
           blocksMovement())
                return true;
        else ;
    }
     
    return false;
}

bool StudentWorld::actorAtXYTakesPeaHit(double x, double y){
    if(actorAt(x, y) != *m_actors.end()){
        if(actorAt(x, y) -> takesPeaHit()) return true;
    }
    return false;
}

//ACTOR AT X,Y BUT DOESN'T BLOCK MOVEMENT
bool StudentWorld::actorNotBlockingAtXY(double x, double y)
{
    list<Actor*>::iterator it;
    if(m_avatar->getX() == x && m_avatar->getY() == y)
        return true;
    for(it = m_actors.begin(); it != m_actors.end(); it++)
    {
        if((*it)->getX() == x && (*it)->getY() == y && !(*it) -> blocksMovement())
                return true;
        else ;
    }
     
    return false;
}

Actor* StudentWorld::actorAt(double x, double y)
{
    list<Actor*>::iterator it;
    if(m_avatar->getX()  == x && m_avatar->getY() == y)
        return m_avatar;
    for(it = m_actors.begin(); it != m_actors.end(); it++)
    {
        if((*it)->getX() == x && (*it)->getY() == y ){
            if((*it)->blocksMovement())
                return *it;
        }
    }
    return *m_actors.end();
}

Actor* StudentWorld::goodieAt(double x, double y){
    list<Actor*>::iterator it;
    if(m_avatar->getX()  == x && m_avatar->getY() == y)
        return m_avatar;
    for(it = m_actors.begin(); it != m_actors.end(); it++)
    {
        if((*it)->getX() == x && (*it)->getY() == y ){
            if(!(*it)->blocksMovement())
                return *it;
        }
    }
    return *m_actors.end();
}
bool StudentWorld::actorAtXYisGoodie(double x, double y){
    if(actorNotBlockingAtXY(x, y)){
        if(goodieAt(x, y)->isGoodie()){
            return true;
        }
    }
    return false;
}

void StudentWorld::removeDeadGameObjects()
{
    list<Actor*>::iterator it;
    for(it = m_actors.begin(); it != m_actors.end(); it++)
    {
        if((*it)->isActive() == false)
        {
            (*it) ->setVisible(false);
            delete *it;
            it = m_actors.erase(it);
        }
    }
}

string StudentWorld::format(int score, int level, int lives, int health, int ammo, int bonus)
{
    string formattedOutput = "";
    ostringstream scoreOSS, levelOSS, livesOSS, healthOSS, ammoOSS, bonusOSS;
    scoreOSS << setw(7) << setfill('0') << score;
    levelOSS << setw(2) << setfill('0') << level;
    livesOSS << setw(2) << lives;
    healthOSS << setw(3) << health;
    ammoOSS << setw(3) << ammo;
    bonusOSS << setw(4) << bonus;
    formattedOutput += "Score: " + scoreOSS.str() + "  Level: " + levelOSS.str() + "  Lives: " + livesOSS.str() + "  Health:" + healthOSS.str() + "%" + "  Ammo: " + ammoOSS.str() + "  Bonus: " + bonusOSS.str();
    return formattedOutput;
    
}

void StudentWorld::setDisplayText()
{
    int score = getScore();
    int level = getLevel();
    unsigned int bonus = getBonus();
    int lives = getLives();
    int health = m_avatar->getHealth();
    int ammo = m_avatar->getAmmo();
    string s = format(score, level, lives, health, ammo, bonus);
    setGameStatText(s);
}

void StudentWorld::addPea(int x, int y, int dir) { m_actors.push_front(new Pea(this, x, y, dir)); }


bool StudentWorld::anyCrystals() const{
    if(m_numCrystals > 0) return true;
    return false;
}
void StudentWorld::decCrystals(){
    m_numCrystals--;
}

int StudentWorld::getAvatarDir(){
    return m_avatar->getDirection();
}

int StudentWorld::getBonus() { return bonusPts; }
double StudentWorld::getAvatarX() { return m_avatar -> getX(); }
double StudentWorld::getAvatarY() { return m_avatar -> getY(); }
void StudentWorld::restorePlayerHealth() { m_avatar->restoreHealth(); }
void StudentWorld::restorePlayerPeas() { m_avatar-> restorePeas(); }
