
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
                    case Level::player:
                        m_avatar = new Avatar(this, x, y);
                        break;
                    case Level::wall:
                        m_actors.push_back(new Wall(this, x, y));
                        break;
                    case Level::crystal:
                        m_actors.push_back(new Crystal(this, x, y));
                        break;
                    case Level::extra_life:
                        m_actors.push_back(new ExtraLifeGoodie(this, x, y));
                        break;
                    case Level::restore_health:
                        m_actors.push_back(new RestoreHealthGoodie(this, x, y));
                        break;
                    case Level::ammo:
                        m_actors.push_back(new AmmoGoodie(this, x, y));
                        break;
                    case Level::marble:
                        m_actors.push_back(new Marble(this, x, y));
                        break;
                    case Level::horiz_ragebot:
                        m_actors.push_back(new RageBot(this, x, y, Alive::right));
                        break;
                    case Level::vert_ragebot:
                        m_actors.push_back(new RageBot(this, x, y, Alive::down));
                        break;
                               
                }
            }
        }
        
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you type q

    //setGameStatText("Game will end when you type q");
    m_avatar -> doSomething();
    list<Actor*>::iterator it;
    for(it = m_actors.begin(); it != m_actors.end(); it++){
        if((*it)->isActive())
        {
            (*it) -> doSomething();
            if(!getAvatar()->isActive())
                return GWSTATUS_PLAYER_DIED;
            //TODO: IMPLEMENT IF PLAYER COMPLETES LEVEL
        }
    }
    
    removeDeadGameObjects();
    
    if(bonusPts > 0) bonusPts--;
    
    //TODO: EXPOSE EXIT ONCE EVERYTHING COMPLETED
    
    //TODO: UPDATE STATUS
    
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

bool StudentWorld::isBlockingAt(double x, double y){
    list<Actor*>::iterator it;
    if(m_avatar->getX() == x && m_avatar->getY() == y) return true;
    for(it = m_actors.begin(); it != m_actors.end(); it++){
        if((*it)->getX() == x && (*it)->getY() == y){
           if((*it) -> blocksMovement())
            return true;
        }
        else ;
    }
    return false;
}

void StudentWorld::removeDeadGameObjects(){
    list<Actor*>::iterator it;
    for(it = m_actors.begin(); it != m_actors.end(); it++){
        if((*it)->isActive() == false)
        {
            (*it) ->setVisible(false);
            it = m_actors.erase(it);
        }
    }
}

Actor* StudentWorld::actorAt(double x, double y){
    std::list<Actor*>::iterator it;
    if(m_avatar->getX()  == x && m_avatar->getY() == y) return m_avatar;
    for(it = m_actors.begin(); it != m_actors.end(); it++){
        if((*it)->getX() == x && (*it)->getY() && (*it)->blocksMovement())
            return *it;
    }
    return *m_actors.end();
}
