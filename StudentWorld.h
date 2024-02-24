
#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include <string>
#include <list>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Actor;
class Avatar;
class Wall;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
    
    //----------------------------------------------------------HELPER FUNCTIONS----------------------------------------------------------

    bool actorIsBlockingAt(double x, double y);
    bool actorNotBlockingAt(double x, double y); //returns if there is an actor that exists but isn't blockign the road
    Avatar* getAvatar();
    std::list<Actor*>& getActors();
    Actor* actorAt(double x, double y);
    void removeDeadGameObjects();
    int getBonus();
    std::string format(int score, int level, int lives, int health, int ammo, int bonus);
    void setDisplayText();
    void addPea(int x, int y, int dir);

    
private:
    Avatar* m_avatar;
    std::list<Actor*> m_actors;
    int bonusPts;
};

#endif // STUDENTWORLD_H_

