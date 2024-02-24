
#include "Actor.h"
#include "StudentWorld.h"
using namespace std;

//---------------------------------------------------ACTOR BASECLASS---------------------------------------------------

Actor::Actor(StudentWorld* sw, int imageID, double x, double y, int dir, bool vis)
:GraphObject(imageID, x, y, dir), m_world(sw), m_isActive(true)
{
    if(vis)
        setVisible(true);
    else
        setVisible(false);
}
Actor::~Actor(){}

StudentWorld* Actor:: getWorld() const
{
    return m_world;
}

bool Actor::isActive()
{
    return m_isActive;
}

void Actor::setActiveState(bool active)
{
    m_isActive = active;
}

bool Actor::isWall() const
{
    return false;
}

bool Actor::isFactory() const
{
    return false;
}

bool Actor::isAvatar() const
{
    return false;
}

bool Actor::isRobot() const
{
    return false;
}

bool Actor::isMarble() const
{
    return false;
}

bool Actor::isPit() const
{
    return false;
}

void Actor::takeDamage(int damage) { return; }


void Actor::moveForward()
{
    if(getDirection() == right)
    {
        if(!(getWorld()->actorIsBlockingAt(getX()+1, getY())))
            moveTo(getX()+1, getY());
        else
            //setDirection(left);
            reactToObstruction();
    }
    else if(getDirection() == left)
    {
        if(!(getWorld()->actorIsBlockingAt(getX()-1, getY())))
            moveTo(getX()-1, getY());
        else
            //setDirection(right);
            reactToObstruction();
    }
    
    else if(getDirection() == up)
    {
        if(!(getWorld()->actorIsBlockingAt(getX(), getY()+1)))
            moveTo(getX(), getY()+1);
        else
           // setDirection(down);
            reactToObstruction();
    }
    else if(getDirection() == down)
    {
        if(!(getWorld()->actorIsBlockingAt(getX(), getY()-1)))
            moveTo(getX(), getY()-1);
        else 
            //setDirection(up);
            reactToObstruction();
        
    }
}


//---------------------------------------------------WALL---------------------------------------------------

Wall::Wall(StudentWorld* sw, double x, double y) : Actor(sw, IID_WALL, x, y, none, true){ }

Wall::~Wall(){}

void Wall::doSomething() 
{
    return;
}

bool Wall::blocksMovement()
{
    return true;
}

bool Wall::isWall() const
{
    return true;
}

void Wall::reactToObstruction()
{
    return;
}



//---------------------------------------------------PIT---------------------------------------------------

Pit::Pit(StudentWorld* sw, double x, double y) : Actor(sw, IID_PIT, x, y, none, true) { }

Pit::~Pit() { }

void Pit::doSomething()
{
    if(!isActive())
        return;
    if(getWorld()->actorAt(getX(), getY())->isMarble())
    {
        setActiveState(false);
        getWorld()->actorAt(getX(), getY())->setActiveState(false);
    }
}

bool Pit::blocksMovement() { return true; }

bool Pit::isPit() const { return true; }

void Pit::reactToObstruction() { return; }

//---------------------------------------------------PEAS---------------------------------------------------

Pea::Pea(StudentWorld* sw, double x, double y, int dir) : Actor(sw, IID_PEA, x, y, dir, true) { }

Pea::~Pea() { }

bool Pea::blocksMovement()
{
    return false;
}

void Pea::reactToObstruction()
{
    setActiveState(false);
}

void Pea::doSomething()
{
    
     /*NOT WORKING IDK WHY
     
   // moveForward();
    if(!isActive()) return;
    if(getWorld()->actorIsAt(getX(), getY()))
    {
    Actor* actor = getWorld()->actorAt(getX(), getY()); --> dont think this is working
    //NOT ENTERING IN THIS IF STATEMENT I DONT KNOW WHY
        if(actor->isRobot() || actor->isAvatar() || actor->isMarble())
        {
            actor->takeDamage(2);
            setActiveState(false);
        }
        else if(actor->isWall() || actor->isFactory())
            setActiveState(false);
    }
    
    moveForward();
    
    if(getWorld()->actorIsAt(getX(), getY()))
    {
        Actor* actor = getWorld()->actorAt(getX(), getY());
        if(actor->isRobot() || actor->isAvatar() || actor->isMarble())
        {
            actor->takeDamage(2);
            setActiveState(false);
        }
        
        else if(actor->isWall() || actor->isFactory())
            setActiveState(false);
    }
     
    */
}


//---------------------------------------------------GOODIES---------------------------------------------------

//---------------------------------------------------EXTRA LIFE GOODIE---------------------------------------------------

ExtraLifeGoodie::ExtraLifeGoodie(StudentWorld* sw, double x, double y) : Actor(sw, IID_EXTRA_LIFE, x, y, none, true) { }

ExtraLifeGoodie::~ExtraLifeGoodie() { }

bool ExtraLifeGoodie::blocksMovement(){
    return false;
}

void ExtraLifeGoodie::reactToObstruction()
{
    return;
}

void ExtraLifeGoodie::doSomething(){
    if(!isActive()) return;
    Avatar* player = getWorld()->getAvatar();
    if(player->getX() == getX() && player->getY() == getY())
    {
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->increaseScore(1000);
        getWorld()->incLives();
        setActiveState(false);
    }
    
}



//---------------------------------------------------RESTORE HEALTH GOODIE-----------------------------------------------

RestoreHealthGoodie::RestoreHealthGoodie(StudentWorld* sw, double x, double y) : Actor(sw, IID_RESTORE_HEALTH, x, y, none, true) { }
RestoreHealthGoodie::~RestoreHealthGoodie() { }
void RestoreHealthGoodie::doSomething()
{
    if(!isActive()) return;
    Avatar* player = getWorld()->getAvatar();
    if(player->getX() == getX() && player->getY() == getY())
    {
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->increaseScore(500);
        player->restoreHealth();
        setActiveState(false);
    }
    
}
bool RestoreHealthGoodie::blocksMovement()
{
    return false;
}

void RestoreHealthGoodie::reactToObstruction()
{
    return;
}


//---------------------------------------------------AMMO GOODIE-----------------------------------------------

AmmoGoodie::AmmoGoodie(StudentWorld* sw, double x, double y) : Actor(sw, IID_AMMO, x, y, none, true) { }

AmmoGoodie::~AmmoGoodie() {}
void AmmoGoodie::doSomething()
{
    if(!isActive()) return;
    Avatar* player = getWorld()->getAvatar();
    if(player->getX() == getX() && player->getY() == getY())
    {
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->increaseScore(100);
        player->restorePeas();
        setActiveState(false);
    }
    
}
bool AmmoGoodie::blocksMovement()
{
    return false;
}

void AmmoGoodie::reactToObstruction()
{
    return;
}


//---------------------------------------------------CRYSTAL---------------------------------------------------

Crystal::Crystal(StudentWorld* sw, double x, double y) : Actor(sw, IID_CRYSTAL, x, y, none, true) { }

Crystal::~Crystal() { }

bool Crystal::blocksMovement()
{
    return false;
}

void Crystal::doSomething()
{
    if(!isActive()) return;
    Avatar* player = getWorld()->getAvatar();
    if(player->getX() == getX() && player->getY() == getY())
    {
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->increaseScore(50);
        setActiveState(false);
    }
}

void Crystal::reactToObstruction()
{
    return;
}


//---------------------------------------------------ALIVE BASECLASS---------------------------------------------------

Alive::Alive(StudentWorld* sw, int imageID, double x, double y, int dir, int hp)
    : Actor(sw, imageID, x, y, dir, true), m_hp(hp){ }
    
Alive::~Alive(){}

void Alive::takeDamage(int damage)
{
    m_hp -= damage;
    playDamageSoundEffect();
    if(m_hp <= 0)
    {
        setDead();
        playDeadSoundEffect();
    }
    else return;
}
    
void Alive::restoreHealth()
{
    m_hp = 20;
}

bool Alive::isAlive()
{
if(m_hp <= 0)
    setDead();
return isActive();
}

void Alive::setDead()
{
    setActiveState(false);
}
    
int Alive::getHP()
{
    return m_hp;
}

//---------------------------------------------------AVATAR---------------------------------------------------
Avatar::Avatar(StudentWorld* sw, double x, double y) : Alive(sw, IID_PLAYER, x, y, right, 20), m_peaCount(20){ }
Avatar::~Avatar(){}

bool Avatar::blocksMovement()
{
    return true;
}

void Avatar::restorePeas()
{
    m_peaCount = 20;
}

void Avatar::doSomething()
{
    if(isAlive() == false)
        return;
    else
    {
        int ch;
        if (getWorld()->getKey(ch))
        {
        // user hit a key this tick! switch (ch)
            switch(ch)
            {
                case KEY_PRESS_ESCAPE:
                    setDead();
                    getWorld()->decLives();
                    break;
                case KEY_PRESS_LEFT:
                    setDirection(left);
                    //if no wall
                    //if(!(getWorld()->isBlockingAt(getX()-1, getY())))
                    //    moveTo(getX()-1, getY());
                    moveForward();
                    break;
                    
                case KEY_PRESS_RIGHT:
                    setDirection(right);
                    //if no wall
                   // if(!(getWorld()->isBlockingAt(getX()+1, getY())))
                    //    moveTo(getX()+1, getY());
                    moveForward();
                    break;
                case KEY_PRESS_UP:
                    setDirection(up);
                    //if no wall
                    //if(!(getWorld()->isBlockingAt(getX(), getY()+1)))
                    //    moveTo(getX(), getY()+1);
                    moveForward();
                    break;
                case KEY_PRESS_DOWN:
                    setDirection(down);
                    //if no wall
                    //if(!(getWorld()->isBlockingAt(getX(), getY()-1)))
                    //    moveTo(getX(), getY()-1);
                    moveForward();
                    break;
                case KEY_PRESS_SPACE:
                    shootPea();
                default:
                    break;
            }
        }
    }
}

void Avatar::shootPea(){
    int x = getX();
    int y = getY();
    int dir = getDirection();
    if(dir == right) x += 1;
    else if(dir == left) x -= 1;
    else if (dir == down) y -= 1;
    else if(dir == up) y += 1;
    getWorld()->addPea(x, y, dir);
    getWorld()->playSound(SOUND_PLAYER_FIRE);
    m_peaCount--;
}

int Avatar::getHealth()
{
    return getHP() * 5;
}

int Avatar::getAmmo()
{
    return m_peaCount;
}
       
bool Avatar::isAvatar()
{
    return true;
}

void Avatar::playDeadSoundEffect()
{
    getWorld()->playSound(SOUND_PLAYER_DIE);
}

void Avatar::playDamageSoundEffect()
{
    getWorld()->playSound(SOUND_PLAYER_IMPACT);
}

void Avatar::reactToObstruction()
{
    int dir = getDirection();
    if(dir == right && getWorld()->actorAt(getX() + 1, getY())->isMarble()){
        getWorld()->actorAt(getX()+1, getY())->moveForward();
        moveTo(getX()+1, getY());
    }
    else if(dir == left && getWorld()->actorAt(getX() - 1, getY())->isMarble()){
        getWorld()->actorAt(getX()-1, getY())->moveForward();
        moveTo(getX()-1, getY());
    }
    else if(dir == up && getWorld()->actorAt(getX(), getY() + 1)->isMarble()){
        getWorld()->actorAt(getX(), getY() + 1)->moveForward();
        moveTo(getX(), getY() + 1);
    }
    else if(dir == down && getWorld()->actorAt(getX(), getY() - 1)->isMarble()){
        getWorld()->actorAt(getX(), getY() - 1)->moveForward();
        moveTo(getX(), getY() - 1);
    }
}


//---------------------------------------------------MARBLE---------------------------------------------------

Marble::Marble(StudentWorld* sw, double x, double y) : Alive(sw, IID_MARBLE, x, y, none, 10) { }

Marble::~Marble(){}


void Marble::doSomething()
{
    return;
}
bool Marble::blocksMovement()
{ 
    return true;
}


void Marble::playDeadSoundEffect()
{
    return;
}

void Marble::playDamageSoundEffect()
{
    return;
}

bool Marble::isMarble()
{
    return true;
}

void Marble::reactToObstruction()
{
    int dir = getDirection();
    //check that there also isn't a goodie that would be blocking the marbles way
    if(dir == right && getWorld()->actorAt(getX() + 1, getY())->isPit() && !getWorld()->actorNotBlockingAt(getX() + 1, getY())){
        moveTo(getX()+1, getY());
    }
    else if(dir == left && getWorld()->actorAt(getX() - 1, getY())->isPit() && !getWorld()->actorNotBlockingAt(getX() - 1, getY())){
        moveTo(getX()-1, getY());
    }
    else if(dir == up && getWorld()->actorAt(getX(), getY() + 1)->isPit()  && !getWorld()->actorNotBlockingAt(getX(), getY() + 1)){
        moveTo(getX(), getY() + 1);
    }
    else if(dir == down && getWorld()->actorAt(getX(), getY() - 1)->isPit() && !getWorld()->actorNotBlockingAt(getX(), getY() -1)){
        moveTo(getX(), getY() - 1);
    }
}


//---------------------------------------------------RAGEBOTS---------------------------------------------------
RageBot::RageBot(StudentWorld* sw, double x, double y, int dir) : Alive(sw, IID_RAGEBOT, x, y, dir, 10)
{
    setDirection(dir);
}

RageBot::~RageBot() { }

bool RageBot::blocksMovement() {return true;}

int RageBot::tickRest() {
    int ticks = (28 - getWorld()->getLevel()) / 4;
    
    if(ticks < 3) ticks = 3;

    return ticks;
}

bool RageBot::facingPlayer(){
    double playerX = getWorld()->getAvatar()->getX();
    double playerY = getWorld()->getAvatar()->getY();
    if(getDirection() == right && playerX > getX()){
        return true;
    }
    else if(getDirection() == left && playerX < getX()){
        return true;
    }
    else if(getDirection() == up && playerY > getY()){
        return true;
    }
    else if(getDirection() == down && playerY < getY()){
        return true;
    }
    return false;
}

bool RageBot::inRange(){
    double playerX = getWorld()->getAvatar()->getX();
    double playerY = getWorld()->getAvatar()->getY();
    if(getDirection() == right || getDirection() == left){
        if(getY() == playerY) { return true;}
    }
    else if(getDirection() == up || getDirection() == down){
        if(getX() == playerX) { return true; }
    }
    return false;
}

bool RageBot::cantShoot()
{
    if(getDirection() == right)
    {
        for(double x = getX() + 1; x < getWorld()->getAvatar()->getX(); x++)
        {
            if(getWorld()->actorIsBlockingAt(x, getY()))
            {
                return true;
            }
        }
    }
    else if(getDirection() == left)
    {
        for(double x = getWorld()->getAvatar()->getX() + 1; x < getX(); x++)
        {
            if(getWorld()->actorIsBlockingAt(x, getY()))
            {
                return true;
            }
        }
    }
    else if(getDirection() == up)
    {
        for(double y = getY() + 1; y < getWorld()->getAvatar()->getY(); y++)
        {
            if(getWorld()->actorIsBlockingAt(getX(), y))
            {
                return true;
            }
        }
    }
    else if(getDirection() == down)
    {
        for(double y = getWorld()->getAvatar()->getY() + 1; y < getY(); y++)
        {
            if(getWorld()->actorIsBlockingAt(getX(), y))
            {
                return true;
            }
        }
    }
    
    return false;
    
}

void RageBot::shootPea(){
    int x = getX();
    int y = getY();
    int dir = getDirection();
    if(dir == right) x += 1;
    else if(dir == left) x -= 1;
    else if (dir == down) y -= 1;
    else if(dir == up) y += 1;
    getWorld()->addPea(x, y, dir);
    getWorld()->playSound(SOUND_ENEMY_FIRE);
}


void RageBot::doSomething()
{

    if(!isAlive())
        return;
    else
    {
        numTicks++;
        if((numTicks + 1) % tickRest() == 0)
        {
            if(inRange() && facingPlayer()){
                if(!cantShoot()){
                    shootPea();
                    //DOESN'T PLAY SHOOTING SOUND BECAUSE OF THIS LINE FOR SOME REASON??
                    //getWorld()->getAvatar()->takeDamage(2);
                }
               else
                   moveForward();
            }
            else
                moveForward();
        }
    }
}

bool RageBot::isRobot()
{
    return true;
}

void RageBot::reactToObstruction()
{
    if(getDirection() == right)
        setDirection(left);
    else if(getDirection() == left)
        setDirection(right);
    else if(getDirection() == up)
        setDirection(down);
    else if(getDirection() == down)
        setDirection(up);
    else return;
}

void RageBot::playDeadSoundEffect()
{
    getWorld()->playSound(SOUND_ROBOT_DIE);
}

void RageBot::playDamageSoundEffect()
{ 
    getWorld()->playSound(SOUND_ROBOT_IMPACT);
}


//---------------------------------------------------THIEFBOTS---------------------------------------------------

