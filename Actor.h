
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
    
    virtual void doSomething() = 0;

    //helper/misc
    void moveForward();       //general move forward function
    virtual void getStolen();

    //getter
    StudentWorld* getWorld() const;
    bool isActive();

    //setter
    void setActiveState(bool active);
    virtual void takeDamage(int damage);
    
    //characteristics
    virtual bool takesPeaHit() const = 0;
    virtual bool blocksMovement() const;
    virtual bool canTakeDamage() const;

    //identifiers
    virtual bool isPit() const;
    virtual bool isMarble() const;
    virtual bool isGoodie() const;
    virtual bool isExtraLife() const;
    virtual bool isRestoreHealth() const;
    virtual bool isAmmo() const;
    virtual bool isFactory() const;
    virtual bool isThiefBot() const;
    virtual bool isAvatar() const;
    virtual bool isExit() const;
    
private:
    StudentWorld* m_world;
    bool m_isActive;
    virtual void reactToObstruction();    //if actor moving forward runs into obstruction
    virtual bool noObstructionExists(double x, double y);    //if the actor moving forward is able to move forward
    
};


//---------------------------------------------------NON-ALIVE---------------------------------------------------

//---------------------------------------------------EXIT---------------------------------------------------
class Exit:public Actor{
public:
    Exit(StudentWorld* sw, double x, double y);
    virtual ~Exit();
    
    virtual void doSomething();
    virtual bool blocksMovement() const;
    virtual bool takesPeaHit() const;
    virtual bool isExit() const;
    
private:
    bool playedSound;
};

//---------------------------------------------------THIEFBOT FACTORY---------------------------------------------------

class ThiefBotFactory: public Actor{
public:
    enum ProductType { REGULAR, MEAN };
    ThiefBotFactory(StudentWorld* sw, double x, double y, ProductType type);
    virtual ~ThiefBotFactory();
    
    virtual void doSomething();
    virtual bool takesPeaHit() const;
    virtual bool isFactory() const;
    int countThiefBots();
    
private:
    ProductType m_type;
    int thiefBotCount;
};
//---------------------------------------------------WALL---------------------------------------------------

class Wall : public Actor
{
public:
    Wall(StudentWorld* sw, double x, double y);
    virtual ~Wall();
    
    virtual void doSomething();
    virtual bool takesPeaHit() const;
    
private:
    
};
//---------------------------------------------------PIT---------------------------------------------------

class Pit : public Actor
{
public:
    Pit(StudentWorld* sw, double x, double y);
    virtual ~Pit();
    
    virtual void doSomething();
    virtual bool isPit() const;
    virtual bool takesPeaHit() const;
    
private:
    
};

//---------------------------------------------------PEA---------------------------------------------------

class Pea : public Actor
{
public:
    Pea(StudentWorld* sw, double x, double y, int dir);
    virtual ~Pea();
    
    void peaHelper();      //checks if something can be hit & take damage if possible
    virtual void doSomething();
    
    virtual bool blocksMovement() const;
    virtual bool takesPeaHit() const;

private:
    virtual void reactToObstruction();
    virtual bool noObstructionExists(double x, double y);
};

//---------------------------------------------------GOODIES---------------------------------------------------
class Goodie : public Actor{
public:
    Goodie(StudentWorld* sw, double x, double y, int imageID, int score);
    virtual ~Goodie();
    
    void doSomething();
    
    virtual bool blocksMovement() const;
    virtual bool takesPeaHit() const;
    virtual bool isGoodie() const;
    virtual void getStolen();
private:
    int m_score;
    //auxiliary
    virtual void goodieDoSomething()=0;
};
//---------------------------------------------------EXTRA LIFE---------------------------------------------------
class ExtraLifeGoodie : public Goodie
{
public:
    ExtraLifeGoodie(StudentWorld* sw, double x, double y);
    virtual ~ExtraLifeGoodie();
    virtual bool isExtraLife() const;
private:
    virtual void goodieDoSomething();
};

//---------------------------------------------------RESTORE HEALTH---------------------------------------------------
class RestoreHealthGoodie : public Goodie
{
public:
    RestoreHealthGoodie(StudentWorld* sw, double x, double y);
    virtual ~RestoreHealthGoodie();
    
    virtual bool isRestoreHealth() const;
private:
    virtual void goodieDoSomething();

};

//---------------------------------------------------AMMO---------------------------------------------------

class AmmoGoodie : public Goodie
{
public:
    AmmoGoodie(StudentWorld* sw, double x, double y);
    virtual ~AmmoGoodie();
    
    virtual bool isAmmo() const;
private:
    virtual void goodieDoSomething();

};

//---------------------------------------------------CRYSTAL---------------------------------------------------

class Crystal : public Goodie
{
public:
    Crystal(StudentWorld* sw, double x, double y);
    virtual ~Crystal();
   
    virtual bool isGoodie() const;
    virtual void getStolen();
    
private:
    virtual void goodieDoSomething();


};
//---------------------------------------------------ALL THINGS ALIVE---------------------------------------------------

//---------------------------------------------------ALIVE BASECLASS---------------------------------------------------

class Alive : public Actor
{
public:
    Alive(StudentWorld* sw, int imageID, double x, double y, int dir, int hp);
    virtual ~Alive();
    
    //getters
    bool isAlive();
    int getHP() const;

    //setters
    void setDead();
    void setHP(int newHP);
    virtual void takeDamage(int damage);
    
    //identifiers
    virtual bool takesPeaHit() const;
    virtual bool canTakeDamage() const;
    
private:
    int m_hp;
    //auxiliary
    virtual void playDamageSoundEffect() = 0;
    virtual void playDeadSoundEffect() = 0;

};

//---------------------------------------------------AVATAR---------------------------------------------------
class Avatar : public Alive
{
public:
    Avatar(StudentWorld* sw, double x, double y);
    virtual ~Avatar();
    
    virtual void doSomething();
    
    //helper
    void shootPea();

    //getters
    int getHealth() const;
    int getAmmo() const;
    
    //setters
    void restoreHealth();
    void restorePeas();
    
    //identifier
    virtual bool isAvatar() const;

private:
    int m_peaCount;
    //auxiliary
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
    virtual bool isMarble() const;

private:
    //aux
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
    
    virtual bool canTakeDamage() const;
    virtual void reactToObstruction();
    virtual void robotDoSomething() = 0;
private:
    int m_score, numTicks;
    virtual void playDamageSoundEffect();
    virtual void playDeadSoundEffect();

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
    ThiefBot(StudentWorld* sw, double x, double y, int hp, int score, int ID);
    virtual ~ThiefBot();
    bool tryToTurn(int dir);
    void takeDamage(int damage);
    virtual void reactToObstruction();
    virtual void robotDoSomething();
    virtual bool isThiefBot() const;

private:
    int distanceBeforeTurning, trackDist;
    bool hasGoodie, hasAmmo, hasHealth, hasLife;
};


//---------------------------------------------------MEAN THIEFBOTS---------------------------------------------------

class MeanThiefBot : public ThiefBot {
public:
    MeanThiefBot(StudentWorld* sw, double x, double y);
    virtual ~MeanThiefBot();
    virtual void robotDoSomething();
private:
    bool shootingPea;
};

#endif // ACTOR_H_
