
#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include <string>
#include <list>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Actor;
class Avatar;
//class Wall;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
    
    //----------------------------------------------------------HELPER FUNCTIONS----------------------------------------------------------

    bool actorIsBlockingAtXY(double x, double y);
    bool actorNotBlockingAtXY(double x, double y); //returns if there is an actor that exists but isn't blockign the road
    std::list<Actor*>& getActors();
    Actor* actorAt(double x, double y);
    Actor* goodieAt(double x, double y);
    void removeDeadGameObjects();
    int getBonus();
    std::string format(int score, int level, int lives, int health, int ammo, int bonus);
    void setDisplayText();
    
    void addPea(int x, int y, int dir);

    double getAvatarX();
    double getAvatarY();
    int getAvatarDir();
    void restorePlayerHealth();
    void restorePlayerPeas();
    
    bool actorAtXYTakesPeaHit(double x, double y);
    bool actorAtXYisGoodie(double x, double y);
    
    
  bool anyCrystals() const;
  void decCrystals();
    
    void addAmmoGoodie(double x, double y);
    void addHealthGoodie(double x, double y);
    void addLifeGoodie(double x, double y);
    void addThiefBot(double x, double y);
    void addMeanThiefBot(double x, double y);
private:
    Avatar* m_avatar;
    std::list<Actor*> m_actors;
    int bonusPts, m_numCrystals;
};

#endif // STUDENTWORLD_H_

