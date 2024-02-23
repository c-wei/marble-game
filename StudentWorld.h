
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
    
    bool isBlockingAt(double x, double y);
    Avatar* getAvatar() { return m_avatar; }
    std::list<Actor*>& getActors() { return m_actors; }
    Actor* actorAt(double x, double y);
    void removeDeadGameObjects();

private:
    Avatar* m_avatar;
    std::list<Actor*> m_actors;
    int bonusPts;
};

#endif // STUDENTWORLD_H_

