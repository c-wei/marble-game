
#include "Actor.h"
#include "StudentWorld.h"
using namespace std;

//---------------------------------------------------ACTOR BASECLASS---------------------------------------------------

Actor::Actor(StudentWorld* sw, int imageID, double x, double y, int dir, bool vis) : GraphObject(imageID, x, y, dir), m_world(sw), m_isActive(true)
{
    if(vis)
        setVisible(true);
    else
        setVisible(false);
}
Actor::~Actor() { }

StudentWorld* Actor:: getWorld() const { return m_world; }
bool Actor::isActive() { return m_isActive; }
void Actor::setActiveState(bool active) { m_isActive = active; }
void Actor::takeDamage(int damage) { return; }
void Actor::reactToObstruction() { return; }
bool Actor::noObstructionExists(double x, double y) {
    return !(getWorld()->actorIsBlockingAtXY(x, y));
}


void Actor::moveForward(){
    if(getDirection() == right)
    {
        if(noObstructionExists(getX()+1, getY()))
            moveTo(getX()+1, getY());
        else
            reactToObstruction();
    }
    else if(getDirection() == left)
    {
        if(noObstructionExists(getX()-1, getY()))
            moveTo(getX()-1, getY());
        else
            reactToObstruction();
    }
    
    else if(getDirection() == up)
    {
        if(noObstructionExists(getX(), getY()+1))
            moveTo(getX(), getY()+1);
        else
            reactToObstruction();
    }
    else if(getDirection() == down)
    {
        if(noObstructionExists(getX(), getY()-1))
            moveTo(getX(), getY()-1);
        else 
            reactToObstruction();
        
    }
}


//---------------------------------------------------WALL---------------------------------------------------

Wall::Wall(StudentWorld* sw, double x, double y) : Actor(sw, IID_WALL, x, y, none, true){ }
Wall::~Wall(){}

void Wall::doSomething() { return; }

bool Wall::blocksMovement() const { return true; }
bool Wall::canTakeDamage() const { return false;}
bool Wall::isMarble() const { return false; }
bool Wall::isPit() const { return false; }
bool Wall::takesPeaDamage() const { return true; }

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

bool Pit::blocksMovement() const { return true; }
bool Pit::canTakeDamage() const { return false; }
bool Pit::isMarble() const { return false; }
bool Pit::isPit() const { return true; }
bool Pit::takesPeaDamage() const { return false; }

//---------------------------------------------------PEAS---------------------------------------------------

Pea::Pea(StudentWorld* sw, double x, double y, int dir) : Actor(sw, IID_PEA, x, y, dir, true) { }
Pea::~Pea() { }

//ONLY SHOWS THE PEA BEING SHOT WHEN THE ACTOR'S TWO STEPS AWAy
void Pea::doSomething(){
    if(!isActive()) return;
    if(getWorld()->actorAtXYTakesPeaDamage(getX(), getY()))
    {
    Actor* actor = getWorld()->actorAt(getX(), getY());
        if(actor->canTakeDamage())
        {
            actor->takeDamage(2);
            setActiveState(false);
        }
        else if(!actor->canTakeDamage())
            setActiveState(false);
    }
    moveForward();
    if(getWorld()->actorAtXYTakesPeaDamage(getX(), getY()))
    {
        Actor* actor = getWorld()->actorAt(getX(), getY());
        if(actor->canTakeDamage())
        {
            actor->takeDamage(2);
            setActiveState(false);
        }
        else if(!actor->canTakeDamage())
            setActiveState(false);
    }
}

void Pea::reactToObstruction(){
    if(getDirection() == right)
    {
        moveTo(getX()+1, getY());
    }
    else if(getDirection() == left)
    {
        moveTo(getX()-1, getY());
    }
    else if(getDirection() == up)
    {
        moveTo(getX(), getY()+1);
    }
    else if(getDirection() == down)
    {
       moveTo(getX(), getY()-1);
    }
    setActiveState(false);
}

bool Pea::noObstructionExists(double x, double y){
    return !getWorld()->actorAtXYTakesPeaDamage(x, y);
}

bool Pea::blocksMovement() const { return false; }
bool Pea::canTakeDamage() const { return false; }
bool Pea::isPit() const { return false; }
bool Pea::isMarble() const { return false; }
bool Pea::takesPeaDamage() const { return false; }

//---------------------------------------------------GOODIES---------------------------------------------------

//---------------------------------------------------EXTRA LIFE GOODIE---------------------------------------------------

ExtraLifeGoodie::ExtraLifeGoodie(StudentWorld* sw, double x, double y) : Actor(sw, IID_EXTRA_LIFE, x, y, none, true) { }
ExtraLifeGoodie::~ExtraLifeGoodie() { }


void ExtraLifeGoodie::doSomething(){
    if(!isActive()) return;
    if(getWorld()->getAvatarX() == getX() && getWorld()->getAvatarY() == getY())
    {
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->increaseScore(1000);
        getWorld()->incLives();
        setActiveState(false);
    }
}

bool ExtraLifeGoodie::blocksMovement() const { return false; }
bool ExtraLifeGoodie::canTakeDamage() const { return false;}
bool ExtraLifeGoodie::isMarble() const { return false; }
bool ExtraLifeGoodie::isPit() const { return false; }
bool ExtraLifeGoodie::takesPeaDamage() const { return false; }

//---------------------------------------------------RESTORE HEALTH GOODIE-----------------------------------------------

RestoreHealthGoodie::RestoreHealthGoodie(StudentWorld* sw, double x, double y) : Actor(sw, IID_RESTORE_HEALTH, x, y, none, true) { }
RestoreHealthGoodie::~RestoreHealthGoodie() { }

void RestoreHealthGoodie::doSomething(){
    if(!isActive()) return;
    if(getWorld()->getAvatarX() == getX() && getWorld()->getAvatarY() == getY())
    {
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->increaseScore(500);
        getWorld()->restorePlayerHealth();
        setActiveState(false);
    }
}

bool RestoreHealthGoodie::blocksMovement() const { return false; }
bool RestoreHealthGoodie::canTakeDamage() const { return false;}
bool RestoreHealthGoodie::isMarble() const { return false; }
bool RestoreHealthGoodie::isPit() const { return false; }
bool RestoreHealthGoodie::takesPeaDamage() const { return false; }

//---------------------------------------------------AMMO GOODIE-----------------------------------------------

AmmoGoodie::AmmoGoodie(StudentWorld* sw, double x, double y) : Actor(sw, IID_AMMO, x, y, none, true) { }
AmmoGoodie::~AmmoGoodie() {}

void AmmoGoodie::doSomething(){
    if(!isActive()) return;
    if(getWorld()->getAvatarX() == getX() && getWorld()->getAvatarY() == getY())
    {
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->increaseScore(100);
        getWorld()->restorePlayerPeas();
        setActiveState(false);
    }
}

bool AmmoGoodie::blocksMovement() const { return false; }
bool AmmoGoodie::canTakeDamage() const { return false;}
bool AmmoGoodie::isMarble() const { return false; }
bool AmmoGoodie::isPit() const { return false; }
bool AmmoGoodie::takesPeaDamage() const { return false; }

//---------------------------------------------------CRYSTAL---------------------------------------------------

Crystal::Crystal(StudentWorld* sw, double x, double y) : Actor(sw, IID_CRYSTAL, x, y, none, true) { }
Crystal::~Crystal() { }

void Crystal::doSomething(){
    if(!isActive()) return;
    if(getWorld()->getAvatarX() == getX() && getWorld()->getAvatarY() == getY())
    {
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->increaseScore(50);
        setActiveState(false);
    }
}

bool Crystal::blocksMovement() const { return false; }
bool Crystal::canTakeDamage() const { return false;}
bool Crystal::isMarble() const { return false; }
bool Crystal::isPit() const { return false; }
bool Crystal::takesPeaDamage() const { return false; }

//---------------------------------------------------ALIVE BASECLASS---------------------------------------------------

Alive::Alive(StudentWorld* sw, int imageID, double x, double y, int dir, int hp) : Actor(sw, imageID, x, y, dir, true), m_hp(hp){ }
Alive::~Alive(){}

void Alive::takeDamage(int damage){
    m_hp -= damage;
    playDamageSoundEffect();
    if(m_hp <= 0)
    {
        setDead();
        playDeadSoundEffect();
    }
    else return;
}

//getters
bool Alive::isAlive(){
if(m_hp <= 0)
    setDead();
return isActive();
}

int Alive::getHP() const { return m_hp; }

//setters
void Alive::setHP(int newHP) { m_hp = newHP; }
void Alive::setDead() { setActiveState(false); }

bool Alive::takesPeaDamage() const { return true; }

//---------------------------------------------------AVATAR---------------------------------------------------
Avatar::Avatar(StudentWorld* sw, double x, double y) : Alive(sw, IID_PLAYER, x, y, right, 20), m_peaCount(20){ }
Avatar::~Avatar(){}

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
                    moveForward();
                    break;
                    
                case KEY_PRESS_RIGHT:
                    setDirection(right);
                    moveForward();
                    break;
                case KEY_PRESS_UP:
                    setDirection(up);
                    moveForward();
                    break;
                case KEY_PRESS_DOWN:
                    setDirection(down);
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

void Avatar::reactToObstruction(){
    int dir = getDirection();
    
    if(dir == right && getWorld()->actorAt(getX() + 1, getY())->isMarble()){
        if(getWorld()->actorIsBlockingAtXY(getX() + 2, getY())){
            if(getWorld()->actorAt(getX() + 2, getY())->isPit()){
                getWorld()->actorAt(getX() + 1, getY())->moveTo(getX()+2, getY());
                moveTo(getX()+1, getY());
            }
        }
        else if(!getWorld()->actorNotBlockingAtXY(getX()+2, getY())){
            getWorld()->actorAt(getX()+1, getY())->moveTo(getX()+2, getY());
            moveTo(getX()+1, getY());
        }
            
    }
    else if(dir == left && getWorld()->actorAt(getX() - 1, getY())->isMarble()){
        if(getWorld()->actorIsBlockingAtXY(getX()-2, getY())){
            if(getWorld()->actorAt(getX() - 2, getY())->isPit()){
                getWorld()->actorAt(getX() - 1, getY())->moveTo(getX()-2, getY());
                moveTo(getX()-1, getY());
            }
        }
        else if(!getWorld()->actorNotBlockingAtXY(getX()+2, getY())){
            getWorld()->actorAt(getX()-1, getY())->moveTo(getX()-2, getY());
            moveTo(getX()-1, getY());
        }
            
    }
    
    else if(dir == up && getWorld()->actorAt(getX(), getY() + 1)->isMarble()){
        if(getWorld()->actorIsBlockingAtXY(getX(), getY()+2)){
            if(getWorld()->actorAt(getX(), getY() + 2)->isPit()){
                getWorld()->actorAt(getX(), getY() + 1)->moveTo(getX(), getY() + 2);
                moveTo(getX(), getY() + 1);
            }
        }
        else if(!getWorld()->actorNotBlockingAtXY(getX(), getY() + 2)){
            getWorld()->actorAt(getX(), getY() + 1)->moveTo(getX(), getY() + 2);
            moveTo(getX(), getY() + 1);
        }
            
    }
    
    else if(dir == down && getWorld()->actorAt(getX(), getY()-1)->isMarble()){
        if(getWorld()->actorIsBlockingAtXY(getX(), getY()-2)){
            if(getWorld()->actorAt(getX(), getY() - 2)->isPit()){
                getWorld()->actorAt(getX(), getY() - 1)->moveTo(getX(), getY() - 2);
                moveTo(getX(), getY() - 1);
            }
        }
        else if(!getWorld()->actorNotBlockingAtXY(getX(), getY() - 2)){
            getWorld()->actorAt(getX(), getY() - 1)->moveTo(getX(), getY() - 2);
            moveTo(getX(), getY() - 1);
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

//getters
int Avatar::getHealth() const { return getHP() * 5; }
int Avatar::getAmmo() const { return m_peaCount; }

//setters
void Avatar::restoreHealth() { setHP(20); }
void Avatar::restorePeas() { m_peaCount = 20; }

bool Avatar::blocksMovement() const { return true; }
bool Avatar::isMarble() const { return false; }
bool Avatar::isPit() const { return false; }
bool Avatar::canTakeDamage() const { return true;}

void Avatar::playDeadSoundEffect() { getWorld()->playSound(SOUND_PLAYER_DIE); }
void Avatar::playDamageSoundEffect() { getWorld()->playSound(SOUND_PLAYER_IMPACT); }


//---------------------------------------------------MARBLE---------------------------------------------------

Marble::Marble(StudentWorld* sw, double x, double y) : Alive(sw, IID_MARBLE, x, y, none, 10) { }
Marble::~Marble(){}


void Marble::doSomething() { return; }

void Marble::reactToObstruction()
{
    int dir = getDirection();
    //TODO: check that there also isn't a goodie that would be blocking the marbles way
    if(dir == right && (getWorld()->actorAt(getX() + 1, getY())->isPit() || getWorld()->actorNotBlockingAtXY(getX() + 1, getY()))){
        moveTo(getX()+1, getY());
    }
    else if(dir == left && (getWorld()->actorAt(getX() - 1, getY())->isPit() || getWorld()->actorNotBlockingAtXY(getX() - 1, getY()))){
        moveTo(getX()-1, getY());
    }
    else if(dir == up && (getWorld()->actorAt(getX(), getY() + 1)->isPit()  || getWorld()->actorNotBlockingAtXY(getX(), getY() + 1))){
        moveTo(getX(), getY() + 1);
    }
    else if(dir == down && (getWorld()->actorAt(getX(), getY() - 1)->isPit() || getWorld()->actorNotBlockingAtXY(getX(), getY() -1))){
        moveTo(getX(), getY() - 1);
    }
}


bool Marble::blocksMovement() const { return true; }
bool Marble::isMarble() const { return true; }
bool Marble::isPit() const { return false; }
bool Marble::canTakeDamage() const { return true;}

void Marble::playDeadSoundEffect() { return; }
void Marble::playDamageSoundEffect() { return; }

//---------------------------------------------------RAGEBOTS---------------------------------------------------
RageBot::RageBot(StudentWorld* sw, double x, double y, int dir) : Alive(sw, IID_RAGEBOT, x, y, dir, 10)  { setDirection(dir); }
RageBot::~RageBot() { }

int RageBot::tickRest() {
    int ticks = (28 - getWorld()->getLevel()) / 4;
    
    if(ticks < 3) ticks = 3;

    return ticks;
}

void RageBot::doSomething(){

    if(!isAlive())
        return;
    else
    {
        numTicks++;
        if((numTicks + 1) % tickRest() == 0)
        {
            if(inRange() && facingPlayer())
            {
                if(!cantShoot())
                    shootPea();
               else
                   moveForward();
            }
            else
                moveForward();
        }
    }
}

bool RageBot::inRange(){
    double playerX = getWorld()->getAvatarX();
    double playerY = getWorld()->getAvatarY();
    if(getDirection() == right || getDirection() == left){
        if(getY() == playerY) { return true;}
    }
    else if(getDirection() == up || getDirection() == down){
        if(getX() == playerX) { return true; }
    }
    return false;
}

bool RageBot::facingPlayer(){
    double playerX = getWorld()->getAvatarX();
    double playerY = getWorld()->getAvatarY();
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

bool RageBot::cantShoot(){
    if(getDirection() == right)
    {
        for(double x = getX() + 1; x < getWorld()->getAvatarX(); x++)
        {
            if(getWorld()->actorIsBlockingAtXY(x, getY()))
            {
                return true;
            }
        }
    }
    else if(getDirection() == left)
    {
        for(double x = getWorld()->getAvatarX() + 1; x < getX(); x++)
        {
            if(getWorld()->actorIsBlockingAtXY(x, getY()))
            {
                return true;
            }
        }
    }
    else if(getDirection() == up)
    {
        for(double y = getY() + 1; y < getWorld()->getAvatarY(); y++)
        {
            if(getWorld()->actorIsBlockingAtXY(getX(), y))
            {
                return true;
            }
        }
    }
    else if(getDirection() == down)
    {
        for(double y = getWorld()->getAvatarY() + 1; y < getY(); y++)
        {
            if(getWorld()->actorIsBlockingAtXY(getX(), y))
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

void RageBot::reactToObstruction() {
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

bool RageBot::blocksMovement() const { return true; }
bool RageBot::canTakeDamage() const { return true; }
bool RageBot::isMarble() const { return false; }
bool RageBot::isPit() const { return false; }

void RageBot::playDeadSoundEffect() { getWorld()->playSound(SOUND_ROBOT_DIE); }
void RageBot::playDamageSoundEffect() { getWorld()->playSound(SOUND_ROBOT_IMPACT); }


//---------------------------------------------------THIEFBOT FACTOR---------------------------------------------------

//---------------------------------------------------THIEFBOTS---------------------------------------------------

