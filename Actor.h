
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
    virtual void takeDamage(int damage);

    void moveForward();

    virtual void doSomething() = 0;
    virtual bool isPit() const = 0;
    virtual bool isMarble() const = 0;
    virtual bool blocksMovement() const = 0;
    virtual bool canTakeDamage() const = 0;
    virtual bool takesPeaDamage() const = 0;
   
private:
    StudentWorld* m_world;
    bool m_isActive;
    virtual void reactToObstruction();
    virtual bool noObstructionExists(double x, double y);
};

//---------------------------------------------------NON-ALIVE---------------------------------------------------

//---------------------------------------------------WALL---------------------------------------------------

class Wall : public Actor
{
public:
    Wall(StudentWorld* sw, double x, double y);
    virtual ~Wall();
    
    virtual void doSomething();
    virtual bool blocksMovement() const;
    virtual bool canTakeDamage() const;
    virtual bool isPit() const;
    virtual bool isMarble() const;
    virtual bool takesPeaDamage() const;
    
private:
    
};
//---------------------------------------------------PIT---------------------------------------------------

class Pit : public Actor
{
public:
    Pit(StudentWorld* sw, double x, double y);
    virtual ~Pit();
    
    virtual void doSomething();
    virtual bool blocksMovement() const;
    virtual bool canTakeDamage() const;
    virtual bool isPit() const;
    virtual bool isMarble() const;
    virtual bool takesPeaDamage() const;
    
private:
    
};

//---------------------------------------------------PEA---------------------------------------------------

class Pea : public Actor
{
public:
    Pea(StudentWorld* sw, double x, double y, int dir);
    virtual ~Pea();
    
    
    virtual void doSomething();                    //continutes moving until it hits smth and possibly makes damage
    
    virtual bool blocksMovement() const;
    virtual bool canTakeDamage() const;
    virtual bool isPit() const;
    virtual bool isMarble() const;
    virtual bool takesPeaDamage() const;

private:
    virtual void reactToObstruction();
    virtual bool noObstructionExists(double x, double y);
};

//---------------------------------------------------GOODIES---------------------------------------------------
//---------------------------------------------------EXTRA LIFE---------------------------------------------------

class ExtraLifeGoodie : public Actor
{
public:
    ExtraLifeGoodie(StudentWorld* sw, double x, double y);
    virtual ~ExtraLifeGoodie();
    
    virtual void doSomething();
    
    virtual bool blocksMovement() const;
    virtual bool canTakeDamage() const;
    virtual bool isPit() const;
    virtual bool isMarble() const;
    virtual bool takesPeaDamage() const;
    
private:
    
};

//---------------------------------------------------RESTORE HEALTH---------------------------------------------------

class RestoreHealthGoodie : public Actor
{
public:
    RestoreHealthGoodie(StudentWorld* sw, double x, double y);
    virtual ~RestoreHealthGoodie();
    
    virtual void doSomething();
    
    virtual bool blocksMovement() const;
    virtual bool canTakeDamage() const;
    virtual bool isPit() const;
    virtual bool isMarble() const;
    virtual bool takesPeaDamage() const;
    
private:
    
};

//---------------------------------------------------AMMO---------------------------------------------------

class AmmoGoodie : public Actor
{
public:
    AmmoGoodie(StudentWorld* sw, double x, double y);
    virtual ~AmmoGoodie();
    
    virtual void doSomething();
    
    virtual bool blocksMovement() const;
    virtual bool canTakeDamage() const;
    virtual bool isPit() const;
    virtual bool isMarble() const;
    virtual bool takesPeaDamage() const;
    
private:
    
};

//---------------------------------------------------CRYSTAL---------------------------------------------------

class Crystal : public Actor
{
public:
    Crystal(StudentWorld* sw, double x, double y);
    virtual ~Crystal();
    
    virtual void doSomething();
    
    virtual bool blocksMovement() const;
    virtual bool canTakeDamage() const;
    virtual bool isPit() const;
    virtual bool isMarble() const;
    virtual bool takesPeaDamage() const;

private:

};
//---------------------------------------------------ALL THINGS ALIVE---------------------------------------------------

//---------------------------------------------------ALIVE BASECLASS---------------------------------------------------

class Alive : public Actor
{
public:
    Alive(StudentWorld* sw, int imageID, double x, double y, int dir, int hp);
    virtual ~Alive();
    
    void takeDamage(int damage);
    
    //getters
    bool isAlive();
    int getHP() const;

    //setters
    void setDead();
    void setHP(int newHP);
    virtual bool takesPeaDamage() const;
    
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
    
    void shootPea();

    //getters
    int getHealth() const;
    int getAmmo() const;
    
    //setters
    void restoreHealth();
    void restorePeas();
    

    virtual bool isPit() const;
    virtual bool isMarble() const;
    virtual bool blocksMovement() const;
    virtual bool canTakeDamage() const;

private:
    int m_peaCount;
    virtual void playDamageSoundEffect();
    virtual void playDeadSoundEffect();
    virtual void reactToObstruction();


};

//---------------------------------------------------MARBLE---------------------------------------------------

class Marble : public Alive
{
public:
    Marble(StudentWorld* sw, double x, double y);
    virtual ~Marble();
    
    virtual void doSomething();                //does nothing
    //void getPushed(); //move the marble if it's pushed
    
    virtual bool blocksMovement() const;
    virtual bool canTakeDamage() const;
    virtual bool isPit() const;
    virtual bool isMarble() const;

private:
    virtual void playDamageSoundEffect();
    virtual void playDeadSoundEffect();
    virtual void reactToObstruction();

};

//---------------------------------------------------RAGEBOTS---------------------------------------------------

class RageBot : public Alive
{
public:
    
    RageBot(StudentWorld* sw, double x, double y, int dir);
    virtual ~RageBot();
    
    int tickRest();
    
    void doSomething();
    bool inRange();    
    bool facingPlayer();
    void shootPea();
    bool cantShoot();

    virtual bool blocksMovement() const;
    virtual bool canTakeDamage() const;
    virtual bool isPit() const;
    virtual bool isMarble() const;

private:
    virtual void playDamageSoundEffect();
    virtual void playDeadSoundEffect();
    virtual void reactToObstruction();

    int numTicks;
};

//---------------------------------------------------THIEFBOT FACTOR---------------------------------------------------

class ThiefBotFactory : public Actor
{
public:
    ThiefBotFactory(StudentWorld* sw, int imageID, double x, double y, int dir, bool vis, int type);
    virtual ~ThiefBotFactory();
    
    StudentWorld* getWorld() const;
    bool isActive();
    void setActiveState(bool active);
    virtual void takeDamage(int damage);

    void moveForward();

    virtual void doSomething() = 0;
    virtual bool isPit() const = 0;
    virtual bool isMarble() const = 0;
    virtual bool blocksMovement() const = 0;
    virtual bool canTakeDamage() const = 0;
   
private:
    StudentWorld* m_world;
    bool m_isActive;
    virtual void reactToObstruction() = 0;
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
