
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
    
    /*
    virtual bool isWall() const = 0;
    virtual bool isFactory() const = 0;
    virtual bool isAvatar() const = 0;
    virtual bool isRobot() const = 0;
    virtual bool isMarble() const = 0;
    virtual bool isPit() const = 0;
    */
    virtual bool isPit() const = 0;
    virtual bool isMarble() const = 0;

    virtual bool canTakeDamage() = 0;
    
    virtual void takeDamage(int damage);
    void moveForward();
    

    
private:
    StudentWorld* m_world;
    bool m_isActive;
    virtual void reactToObstruction() = 0;
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
    /*
    virtual bool isWall() const;
    virtual bool isFactory() const;
    virtual bool isAvatar() const;
    virtual bool isRobot() const;
    virtual bool isMarble() const;
    virtual bool isPit() const;
    */
    virtual bool isPit() const;
    virtual bool isMarble() const;

    virtual bool canTakeDamage();

private:
    virtual void reactToObstruction();
};
//---------------------------------------------------PIT---------------------------------------------------

class Pit : public Actor
{
public:
    Pit(StudentWorld* sw, double x, double y);
    virtual ~Pit();
    
    
    virtual void doSomething();
    virtual bool blocksMovement();
    /*
    virtual bool isPit() const;
    virtual bool isWall() const;
    virtual bool isFactory() const;
    virtual bool isAvatar() const;
    virtual bool isRobot() const;
    virtual bool isMarble() const;
    */
    virtual bool isPit() const;
    virtual bool isMarble() const;

    virtual bool canTakeDamage();
private:
    virtual void reactToObstruction();
};

//---------------------------------------------------PEA---------------------------------------------------

class Pea : public Actor
{
public:
    Pea(StudentWorld* sw, double x, double y, int dir);
    virtual ~Pea();
    
    
    virtual void doSomething();                    //continutes moving until it hits smth and possibly makes damage
    virtual bool blocksMovement();                     //doesn't obstruct movement
    /*
    virtual bool isWall() const;
    virtual bool isFactory() const;
    virtual bool isAvatar() const;
    virtual bool isRobot() const;
    virtual bool isMarble() const;
    virtual bool isPit() const;
*/
    virtual bool isPit() const;
    virtual bool isMarble() const;

    virtual bool canTakeDamage();

private:
    virtual void reactToObstruction();
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
    
    /*
    virtual bool isWall() const;
    virtual bool isFactory() const;
    virtual bool isAvatar() const;
    virtual bool isRobot() const;
    virtual bool isMarble() const;
    virtual bool isPit() const;
*/
    virtual bool isPit() const;
    virtual bool isMarble() const;

    virtual bool canTakeDamage();
private:
    virtual void reactToObstruction();
};

//---------------------------------------------------RESTORE HEALTH---------------------------------------------------

class RestoreHealthGoodie : public Actor
{
public:
    RestoreHealthGoodie(StudentWorld* sw, double x, double y);
    virtual ~RestoreHealthGoodie();
    virtual void doSomething();
    virtual bool blocksMovement();
    
    /*
    virtual bool isWall() const;
    virtual bool isFactory() const;
    virtual bool isAvatar() const;
    virtual bool isRobot() const;
    virtual bool isMarble() const;
    virtual bool isPit() const;
*/
    virtual bool isPit() const;
    virtual bool isMarble() const;

    virtual bool canTakeDamage();
     
private:
    virtual void reactToObstruction();
};

//---------------------------------------------------AMMO---------------------------------------------------

class AmmoGoodie : public Actor
{
public:
    AmmoGoodie(StudentWorld* sw, double x, double y);
    virtual ~AmmoGoodie();
    virtual void doSomething();
    virtual bool blocksMovement();
    
    /*
    virtual bool isWall() const;
    virtual bool isFactory() const;
    virtual bool isAvatar() const;
    virtual bool isRobot() const;
    virtual bool isMarble() const;
    virtual bool isPit() const;

     */
    virtual bool isPit() const;
    virtual bool isMarble() const;

    virtual bool canTakeDamage();
private:
    virtual void reactToObstruction();
};

//---------------------------------------------------CRYSTAL---------------------------------------------------

class Crystal : public Actor
{
public:
    Crystal(StudentWorld* sw, double x, double y);
    virtual ~Crystal();
    
    virtual void doSomething();
    virtual bool blocksMovement();
    /*
    virtual bool isWall() const;
    virtual bool isFactory() const;
    virtual bool isAvatar() const;
    virtual bool isRobot() const;
    virtual bool isMarble() const;
    virtual bool isPit() const;
*/
    virtual bool isPit() const;
    virtual bool isMarble() const;

    virtual bool canTakeDamage();
private:
    virtual void reactToObstruction();

};
//---------------------------------------------------ALL THINGS ALIVE---------------------------------------------------

//---------------------------------------------------ALIVE BASECLASS---------------------------------------------------

class Alive : public Actor
{
public:
    Alive(StudentWorld* sw, int imageID, double x, double y, int dir, int hp);
    virtual ~Alive();
    void takeDamage(int damage);
    void restoreHealth();
    bool isAlive();
    void setDead();
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
    
    /*
    virtual bool isAvatar() const;
    virtual bool isWall() const;
    virtual bool isFactory() const;
    virtual bool isRobot() const;
    virtual bool isMarble() const;
    virtual bool isPit() const;
     
     */
    virtual bool isPit() const;
    virtual bool isMarble() const;

    virtual bool canTakeDamage();
    void shootPea();

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
    virtual bool blocksMovement(); //obstructs movement
    
    void getPushed(); //move the marble if it's pushed
    
    /*
    virtual bool isMarble() const;
    virtual bool isWall() const;
    virtual bool isFactory() const;
    virtual bool isAvatar() const;
    virtual bool isRobot() const;
    virtual bool isPit() const;
     */
    virtual bool isPit() const;
    virtual bool isMarble() const;

    virtual bool canTakeDamage();
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
    virtual bool blocksMovement();


    virtual ~RageBot();
    
    int tickRest();
    
    
    bool facingPlayer();
    
    bool inRange();
    
    bool cantShoot();
    
    void shootPea();
    
    void doSomething();
    
    /*
    virtual bool isRobot() const;
    virtual bool isWall() const;
    virtual bool isFactory() const;
    virtual bool isAvatar() const;
    virtual bool isMarble() const;
    virtual bool isPit() const;
*/
    virtual bool isPit() const;
    virtual bool isMarble() const;

    virtual bool canTakeDamage();
private:
    virtual void playDamageSoundEffect();
    virtual void playDeadSoundEffect();
    virtual void reactToObstruction();

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
