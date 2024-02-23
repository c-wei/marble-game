
#ifndef ACTOR_H_
#define ACTOR_H_

#include <stdlib.h>

class StudentWorld;
#include "GraphObject.h"

//class StudentWorld;

//---------------------------------------------------ACTOR BASECLASS---------------------------------------------------

class Actor : public GraphObject
{
public:
    Actor(StudentWorld* sw, int imageID, double x, double y, int dir, bool vis);
    virtual ~Actor();
    
    StudentWorld* getWorld() const;
    
    bool isActive();
    
    void setActiveState(bool active);
    
    virtual void doSomething() = 0;
    virtual bool blocksMovement() = 0;
    
    virtual bool isWall() const;
    virtual void takeDamage(int damage);
    virtual void damageEffect();
    bool existingObstacle(double x, double y);
    void moveForward();

    
private:
    StudentWorld* m_world;
    bool m_isActive;
};

//---------------------------------------------------NON-ALIVE---------------------------------------------------

//---------------------------------------------------WALL---------------------------------------------------

class Wall : public Actor
{
public:
    Wall(StudentWorld* sw, double x, double y);
    virtual ~Wall();
    
    virtual void doSomething();
    virtual bool blocksMovement();
    virtual bool isWall() const;
    
private:

};

//---------------------------------------------------PEA---------------------------------------------------

class Pea : public Actor
{
public:
    Pea(StudentWorld* sw, double x, double y, int dir);
    virtual ~Pea();
    
    virtual void doSomething();                    //continutes moving until it hits smth and possibly makes damage
    virtual bool blocksMovement();                     //doesn't obstruct movement

    
private:
    
};

//---------------------------------------------------GOODIES---------------------------------------------------
//---------------------------------------------------EXTRA LIFE---------------------------------------------------

class ExtraLifeGoodie : public Actor
{
public:
    ExtraLifeGoodie(StudentWorld* sw, double x, double y);
    virtual ~ExtraLifeGoodie();
    virtual void doSomething();
    virtual bool blocksMovement();

private:

};

//---------------------------------------------------RESTORE HEALTH---------------------------------------------------

class RestoreHealthGoodie : public Actor
{
public:
    RestoreHealthGoodie(StudentWorld* sw, double x, double y);
    virtual ~RestoreHealthGoodie();
    virtual void doSomething();
    virtual bool blocksMovement();
    
private:
    
};

//---------------------------------------------------AMMO---------------------------------------------------

class AmmoGoodie : public Actor
{
public:
    AmmoGoodie(StudentWorld* sw, double x, double y);
    virtual ~AmmoGoodie();
    virtual void doSomething();
    virtual bool blocksMovement();

private:
    
};

//---------------------------------------------------CRYSTAL---------------------------------------------------

class Crystal : public Actor
{
public:
    Crystal(StudentWorld* sw, double x, double y);
    virtual ~Crystal();
    
    virtual void doSomething();
    virtual bool blocksMovement();

};
//---------------------------------------------------ALL THINGS ALIVE---------------------------------------------------

//---------------------------------------------------ALIVE BASECLASS---------------------------------------------------

class Alive : public Actor
{
public:
    Alive(StudentWorld* sw, int imageID, double x, double y, int dir, int hp);
    virtual ~Alive();
    virtual void takeDamage(int damage);
    void restoreHealth();
    bool isAlive();
    void setDead();
    virtual void damageEffect();
    int getHP();


private:
    virtual void playDamageSoundEffect() = 0;
    virtual void playDeadSoundEffect() = 0;
    int m_hp;

};

//---------------------------------------------------AVATAR---------------------------------------------------
class Avatar : public Alive
{
public:
    Avatar(StudentWorld* sw, double x, double y);
    virtual ~Avatar();
    
    virtual void doSomething();
    virtual bool blocksMovement();
    
    void restorePeas();
    int getHealth();
    int getAmmo();
        
private:
    int m_peaCount;
    virtual void playDamageSoundEffect();
    virtual void playDeadSoundEffect();

};

//---------------------------------------------------MARBLE---------------------------------------------------

class Marble : public Alive
{
public:
    Marble(StudentWorld* sw, double x, double y);
    
    virtual ~Marble();
    
    virtual void doSomething();                //does nothing
    virtual bool blocksMovement(); //obstructs movement
    
    void getPushed(); //move the marble if it's pushed
    
private:
    virtual void playDamageSoundEffect();
    virtual void playDeadSoundEffect();
};

//---------------------------------------------------RAGEBOTS---------------------------------------------------

class RageBot : public Alive
{
public:
    
    RageBot(StudentWorld* sw, double x, double y, int dir);
    virtual bool blocksMovement();


    virtual ~RageBot();
    
    int tickRest();
    
    
    bool facingPlayer();
    
    bool inRange();
    
    bool cantShoot();
    
    void shootPea();
    
    void doSomething();
    
private:
    virtual void playDamageSoundEffect();
    virtual void playDeadSoundEffect();
    int numTicks;
};

//---------------------------------------------------THIEFBOTS---------------------------------------------------

//---------------------------------------------------NORMAL---------------------------------------------------
/*
class ThiefBot : public Robot
{
public:
    ThiefBot(StudentWorld* sw, double x, double y, int roboType)
    : Robot(sw, x, y, IID_THIEFBOT, roboType, 5), distanceBeforeTurning((rand() % 6) + 1){}
    
    
    virtual bool inRange();                    //check if player in robot's direct line
    
    virtual void moveAround();                    //moving back and forth

    void pickUpGoodie();
    
protected:
    int distanceBeforeTurning;
};


*/
#endif // ACTOR_H_
