
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
    virtual void getStolen();
    virtual void getDropped(double x, double y);
    virtual bool isPit() const;
    virtual bool isMarble() const;
    virtual bool blocksMovement() const = 0;
    virtual bool canTakeDamage() const = 0;
    virtual bool takesPeaHit() const = 0;
    virtual bool isGoodie() const;
    
    virtual bool isExtraLife() const;
    virtual bool isRestoreHealth() const;
    virtual bool isAmmo() const;
    
    int getID();
   
private:
    StudentWorld* m_world;
    bool m_isActive;
    virtual void reactToObstruction();
    virtual bool noObstructionExists(double x, double y);
    
    int m_ID;
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
    //virtual bool isPit() const;
    //virtual bool isMarble() const;
    virtual bool takesPeaHit() const;
   // virtual bool isGoodie() const;
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
    //virtual bool isMarble() const;
    virtual bool takesPeaHit() const;
    //virtual bool isGoodie() const;
    
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
    //virtual bool isPit() const;
    //virtual bool isMarble() const;
    virtual bool takesPeaHit() const;
    //virtual bool isGoodie() const;

private:
    virtual void reactToObstruction();
    virtual bool noObstructionExists(double x, double y);
};

//---------------------------------------------------GOODIES---------------------------------------------------
class Goodie : public Actor{
public:
    Goodie(StudentWorld* sw, double x, double y, int imageID);
    virtual ~Goodie();
    virtual bool blocksMovement() const;
    virtual bool canTakeDamage() const;
    virtual bool takesPeaHit() const;
    virtual bool isGoodie() const;
    virtual void getStolen();
private:
};
//---------------------------------------------------EXTRA LIFE---------------------------------------------------
class ExtraLifeGoodie : public Goodie
{
public:
    ExtraLifeGoodie(StudentWorld* sw, double x, double y);
    virtual ~ExtraLifeGoodie();
    virtual void doSomething();
    virtual bool isExtraLife() const;
    
};

//---------------------------------------------------RESTORE HEALTH---------------------------------------------------
class RestoreHealthGoodie : public Goodie
{
public:
    RestoreHealthGoodie(StudentWorld* sw, double x, double y);
    virtual ~RestoreHealthGoodie();
    
    virtual void doSomething();
    virtual bool isRestoreHealth() const;
};

//---------------------------------------------------AMMO---------------------------------------------------

class AmmoGoodie : public Goodie
{
public:
    AmmoGoodie(StudentWorld* sw, double x, double y);
    virtual ~AmmoGoodie();
    
    virtual void doSomething();
    virtual bool isAmmo() const;
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
    //virtual bool isPit() const;
    //virtual bool isMarble() const;
    virtual bool takesPeaHit() const;
    //virtual bool isGoodie() const;
    
private:

};
//---------------------------------------------------ALL THINGS ALIVE---------------------------------------------------

//---------------------------------------------------ALIVE BASECLASS---------------------------------------------------

class Alive : public Actor
{
public:
    Alive(StudentWorld* sw, int imageID, double x, double y, int dir, int hp);
    virtual ~Alive();
    
    virtual void takeDamage(int damage);
    
    //getters
    bool isAlive();
    int getHP() const;

    //setters
    void setDead();
    void setHP(int newHP);
    virtual bool takesPeaHit() const;
    //virtual bool isGoodie() const;
    
    
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
    

   // virtual bool isPit() const;
    //virtual bool isMarble() const;
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
    //virtual bool isPit() const;
    virtual bool isMarble() const;

private:
    virtual void playDamageSoundEffect();
    virtual void playDeadSoundEffect();
    virtual void reactToObstruction();

};
//---------------------------------------------------ROBOTS---------------------------------------------------
class Robot : public Alive
{
public:
    Robot(StudentWorld* sw, double x, double y, int dir, int hp, int score, int imgID);
    virtual ~Robot();
    
    int tickRest();
    virtual void doSomething();
    bool inRange();
    bool facingPlayer();
    bool cantShoot();
    void shootPea();
    
    virtual bool blocksMovement() const;
    virtual bool canTakeDamage() const;
    //virtual bool isPit() const;
   // virtual bool isMarble() const;
    virtual void reactToObstruction();
    virtual void robotDoSomething() = 0;
private:
    int m_score;
    virtual void playDamageSoundEffect();
    virtual void playDeadSoundEffect();

    int numTicks;

};


//---------------------------------------------------RAGEBOTS---------------------------------------------------
class RageBot : public Robot{
public:
    RageBot(StudentWorld* sw, double x, double y, int dir);
    virtual ~RageBot();
    virtual void robotDoSomething();

private:
    int numTicks;
};


//---------------------------------------------------THIEFBOT FACTOR---------------------------------------------------


//---------------------------------------------------THIEFBOTS---------------------------------------------------

class ThiefBot : public Robot{
public:
    ThiefBot(StudentWorld* sw, double x, double y, int dir);
    virtual ~ThiefBot();
    bool tryToTurn(int dir);
    void takeDamage(int damage);
    virtual void reactToObstruction();
    virtual void robotDoSomething();


private:
    int distanceBeforeTurning, trackDist;
    bool hasGoodie, hasAmmo, hasHealth, hasLife;
};


//---------------------------------------------------MEAN THIEFBOTS---------------------------------------------------

class MeanThiefBot : public ThiefBot {
public:
    MeanThiefBot(StudentWorld* sw, double x, double y, int dir);
    virtual ~MeanThiefBot();
    
private:
    virtual void robotDoSomething();
    virtual void reactToObstruction();
};

#endif // ACTOR_H_
