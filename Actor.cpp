
#include "Actor.h"
#include "StudentWorld.h"
using namespace std;

//---------------------------------------------------ACTOR BASECLASS---------------------------------------------------

Actor::Actor(StudentWorld* sw, int imageID, double x, double y, int dir, bool vis) : GraphObject(imageID, x, y, dir), m_world(sw), m_isActive(true), m_ID(imageID)
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
    if((isMarble() && getWorld()->getAvatarDir() == right) || getDirection() == right)
    {
        if(noObstructionExists(getX()+1, getY()))
            moveTo(getX()+1, getY());
        else
            reactToObstruction();
    }
    else if((isMarble() && getWorld()->getAvatarDir() == left) || getDirection() == left)
    {
        if(noObstructionExists(getX()-1, getY()))
            moveTo(getX()-1, getY());
        else
            reactToObstruction();
    }
    
    else if((isMarble() && getWorld()->getAvatarDir() == up) || getDirection() == up)
    {
        if(noObstructionExists(getX(), getY()+1))
            moveTo(getX(), getY()+1);
        else
            reactToObstruction();
    }
    else if((isMarble() && getWorld()->getAvatarDir() == down) || getDirection() == down)
    {
        if(noObstructionExists(getX(), getY()-1))
            moveTo(getX(), getY()-1);
        else 
            reactToObstruction();
        
    }
}

void Actor::getStolen() { return; }
void Actor::getDropped(double x, double y) { return; }

bool Actor::isMarble() const { return false; }
bool Actor::isPit() const { return false; }
bool Actor::isGoodie() const { return false; }

int Actor::getID() { return m_ID; }
bool Actor::isAmmo() const { return false; }
bool Actor::isRestoreHealth() const { return false; }
bool Actor::isExtraLife() const { return false; }
bool Actor::isFactory() const { return false; }
bool Actor::isThiefBot() const { return false; }

//---------------------------------------------------THIEFBOT FACTORY---------------------------------------------------
ThiefBotFactory::ThiefBotFactory(StudentWorld *sw, double x, double y, ProductType type) : Actor(sw, IID_ROBOT_FACTORY, x, y, none, true), m_type(type), thiefBotCount(0){}
ThiefBotFactory::~ThiefBotFactory(){}

void ThiefBotFactory::doSomething(){
    thiefBotCount = 0;
    double checkXLower = getX()-3;
    if(checkXLower < 0) checkXLower = 0;
    double checkXUpper = getX()+3;
    if(checkXUpper>14) checkXUpper = 14;
    double checkYLower = getY() - 3;
    if(checkYLower < 0) checkYLower = 0;
    double checkYUpper = getY() + 3;
    if(checkYUpper > 14) checkYUpper = 14;
    for(double x = checkXLower; x <= checkXUpper; x++){
        for(double y = checkYLower; y <= checkYUpper; y++){
            if(getWorld()->actorIsBlockingAtXY(x, y)){
                if(getWorld()->actorAt(x, y)->isThiefBot())
                    thiefBotCount++;
            }
        }
    }
    int r = randInt(1, 50);
    if(thiefBotCount < 3 && !getWorld()->actorAt(getX(), getY())->isThiefBot() && r == 1){
        if(m_type == REGULAR)
            getWorld() -> addThiefBot(getX(), getY());
        else if(m_type == MEAN)
            getWorld() ->addMeanThiefBot(getX(), getY());
    }
}

bool ThiefBotFactory::blocksMovement() const { return true; }
bool ThiefBotFactory::canTakeDamage() const { return false; }
bool ThiefBotFactory::takesPeaHit() const { return true; }
bool ThiefBotFactory::isFactory() const { return true; }

//---------------------------------------------------WALL---------------------------------------------------

Wall::Wall(StudentWorld* sw, double x, double y) : Actor(sw, IID_WALL, x, y, none, true){ }
Wall::~Wall(){}

void Wall::doSomething() { return; }

bool Wall::blocksMovement() const { return true; }
bool Wall::canTakeDamage() const { return false;}
//bool Wall::isMarble() const { return false; }
//bool Wall::isPit() const { return false; }
bool Wall::takesPeaHit() const { return true; }
//bool Wall::isGoodie() const { return false; }

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
//bool Pit::isMarble() const { return false; }
bool Pit::isPit() const { return true; }
bool Pit::takesPeaHit() const { return false; }
//bool Pit::isGoodie() const { return false; }

//---------------------------------------------------PEAS---------------------------------------------------

Pea::Pea(StudentWorld* sw, double x, double y, int dir) : Actor(sw, IID_PEA, x, y, dir, true) { }
Pea::~Pea() { }

void Pea::doSomething(){
    if(!isActive()) return;
    if(getWorld()->actorAtXYTakesPeaHit(getX(), getY()))
    {
    Actor* actor = getWorld()->actorAt(getX(), getY());
        if(actor->canTakeDamage())
        {
            actor->takeDamage(2);
            setActiveState(false);
        }
        else if(!actor->canTakeDamage()){
            setActiveState(false);
        }
    }
    moveForward();
    if(getWorld()->actorAtXYTakesPeaHit(getX(), getY()))
    {
        Actor* actor = getWorld()->actorAt(getX(), getY());
        if(actor->canTakeDamage())
        {
            actor->takeDamage(2);
            setActiveState(false);
        }
        else if(!actor->canTakeDamage()){
            setActiveState(false);
        }
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
    return !getWorld()->actorAtXYTakesPeaHit(x, y);
}

bool Pea::blocksMovement() const { return false; }
bool Pea::canTakeDamage() const { return false; }
//bool Pea::isPit() const { return false; }
//bool Pea::isMarble() const { return false; }
bool Pea::takesPeaHit() const { return false; }
//bool Pea::isGoodie() const { return false; }

//---------------------------------------------------GOODIES---------------------------------------------------

Goodie::Goodie(StudentWorld* sw, double x, double y, int imageID) : Actor(sw, imageID, x, y, none, true) { }
Goodie::~Goodie() { }
//bool Goodie::canBePickedUp() const { return isPickupable; }
bool Goodie::blocksMovement() const { return false; }
bool Goodie::canTakeDamage() const { return false; }
bool Goodie::takesPeaHit() const { return false; }
bool Goodie::isGoodie() const { return true; }
void Goodie::getStolen() {
    setActiveState(false);
}


//---------------------------------------------------EXTRA LIFE GOODIE---------------------------------------------------
ExtraLifeGoodie::ExtraLifeGoodie(StudentWorld* sw, double x, double y) : Goodie(sw, x, y, IID_EXTRA_LIFE) { }

ExtraLifeGoodie::~ExtraLifeGoodie() {}
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

bool ExtraLifeGoodie::isExtraLife() const { return true; }

//---------------------------------------------------RESTORE HEALTH GOODIE-----------------------------------------------
RestoreHealthGoodie::RestoreHealthGoodie(StudentWorld* sw, double x, double y) : Goodie(sw, x, y, IID_RESTORE_HEALTH){ }
RestoreHealthGoodie::~RestoreHealthGoodie() { setActiveState(false); }

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

bool RestoreHealthGoodie::isRestoreHealth() const { return true; }


//---------------------------------------------------AMMO GOODIE-----------------------------------------------
AmmoGoodie::AmmoGoodie(StudentWorld* sw, double x, double y) : Goodie(sw, x, y, IID_AMMO) { }
AmmoGoodie::~AmmoGoodie() {setActiveState(false);}

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

bool AmmoGoodie::isAmmo() const { return true; }


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
//bool Crystal::isMarble() const { return false; }
//bool Crystal::isPit() const { return false; }
bool Crystal::takesPeaHit() const { return false; }
//bool Crystal::isGoodie() const { return false; }

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

bool Alive::takesPeaHit() const { return true; }

//bool Alive::isGoodie() const { return false; }

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
    if(m_peaCount>0){
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
}

//getters
int Avatar::getHealth() const { return getHP() * 5; }
int Avatar::getAmmo() const { return m_peaCount; }

//setters
void Avatar::restoreHealth() { setHP(20); }
void Avatar::restorePeas() { m_peaCount = 20; }

bool Avatar::blocksMovement() const { return true; }
//bool Avatar::isMarble() const { return false; }
//bool Avatar::isPit() const { return false; }
bool Avatar::canTakeDamage() const { return true;}

void Avatar::playDeadSoundEffect() { getWorld()->playSound(SOUND_PLAYER_DIE); }
void Avatar::playDamageSoundEffect() { getWorld()->playSound(SOUND_PLAYER_IMPACT); }


//---------------------------------------------------MARBLE---------------------------------------------------

Marble::Marble(StudentWorld* sw, double x, double y) : Alive(sw, IID_MARBLE, x, y, none, 10) { }
Marble::~Marble(){}


void Marble::doSomething() { return; }

void Marble::reactToObstruction()
{
    int dir = getWorld()->getAvatarDir();
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
//bool Marble::isPit() const { return false; }
bool Marble::canTakeDamage() const { return true;}

void Marble::playDeadSoundEffect() { return; }
void Marble::playDamageSoundEffect() { return; }

//---------------------------------------------------ROBOTS---------------------------------------------------
Robot::Robot(StudentWorld* sw, double x, double y, int dir, int hp, int score, int imgID) : Alive(sw, imgID, x, y, dir, hp), m_score(score), numTicks(0){ setDirection(dir); }
Robot::~Robot() { }

int Robot::tickRest() {
    int ticks = (28 - getWorld()->getLevel()) / 4;
    
    if(ticks < 3) ticks = 3;

    return ticks;
}

void Robot::doSomething(){

    if(!isAlive())
        return;
    else
    {
        numTicks++;
        if((numTicks + 1) % tickRest() == 0)
        {
            robotDoSomething();
        }
    }
}

bool Robot::inRange(){
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

bool Robot::facingPlayer(){
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

bool Robot::cantShoot(){
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

void Robot::shootPea(){
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

void Robot::reactToObstruction() {
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


bool Robot::blocksMovement() const { return true; }
bool Robot::canTakeDamage() const { return true; }
//bool Robot::isMarble() const { return false; }
//bool Robot::isPit() const { return false; }

void Robot::playDeadSoundEffect() {
    getWorld()->increaseScore(m_score);
    getWorld()->playSound(SOUND_ROBOT_DIE);
}
void Robot::playDamageSoundEffect() { getWorld()->playSound(SOUND_ROBOT_IMPACT); }
//---------------------------------------------------RAGEBOTS---------------------------------------------------
RageBot::RageBot(StudentWorld* sw, double x, double y, int dir) : Robot(sw, x, y, dir, 10, 100, IID_RAGEBOT), numTicks(0) { }
RageBot::~RageBot() {}

void RageBot::robotDoSomething(){
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

//---------------------------------------------------THIEFBOT ---------------------------------------------------

ThiefBot::ThiefBot(StudentWorld* sw, double x, double y, int hp, int score, int ID) : Robot(sw, x, y, right, hp, score, ID) {
    distanceBeforeTurning = randInt(1, 6);
    trackDist = 0;
    hasGoodie = false;
    hasAmmo = false;
    hasHealth = false;
    hasLife = false;
}
ThiefBot::~ThiefBot() { }

void ThiefBot::robotDoSomething(){
    int r = randInt(1, 10);
    if(getWorld()->actorAtXYisGoodie(getX(), getY()) && r == 1 && !hasGoodie){
        Actor* goodie = getWorld()->goodieAt(getX(), getY());
        goodie->getStolen();
        if(goodie->isAmmo())
            hasAmmo = true;
        else if(goodie->isRestoreHealth())
            hasHealth = true;
        else if(goodie->isExtraLife())
            hasLife = true;
        getWorld()->playSound(SOUND_ROBOT_MUNCH);
        hasGoodie = true;
    }
    else if(trackDist < distanceBeforeTurning){
        moveForward();
        if(getDirection() == right && !getWorld()->actorIsBlockingAtXY(getX()+1, getY()))
            trackDist++;
        else if(getDirection() == left && !getWorld()->actorIsBlockingAtXY(getX()-1, getY()))
            trackDist++;
        else if(getDirection() == up && !getWorld()->actorIsBlockingAtXY(getX(), getY()+1))
            trackDist++;
        else if(getDirection() == down && !getWorld()->actorIsBlockingAtXY(getX(), getY()-1))
            trackDist++;
    }
    else if(trackDist >= distanceBeforeTurning){
        distanceBeforeTurning = randInt(1, 6);
        trackDist = 0;
        int dir = randInt(1, 4);
        if(dir == 1) dir = right;
        else if(dir == 2) dir = left;
        else if(dir == 3) dir = up;
        else if(dir == 4) dir = down;
        if(!tryToTurn(dir)) setDirection(dir);
    }
    
    
}

void ThiefBot::reactToObstruction(){
    int dir = randInt(1, 4);
    if(dir == 1) dir = right;
    else if(dir == 2) dir = left;
    else if(dir == 3) dir = up;
    else if(dir == 4) dir = down;
    if(!tryToTurn(dir)) setDirection(dir);
}

bool ThiefBot::tryToTurn(int dir){
    if(dir == right){
        if(!getWorld()->actorIsBlockingAtXY(getX()+1, getY())) {
            setDirection(right);
            moveTo(getX()+1, getY());
            return true;
        }
    }
    else if(dir == left){
        if(!getWorld()->actorIsBlockingAtXY(getX()-1, getY())) {
            setDirection(left);
            moveTo(getX()-1, getY());

            return true;
        }
    }
    else if(dir == up){
        if(!getWorld()->actorIsBlockingAtXY(getX(), getY()+1)) {
            setDirection(up);
            moveTo(getX(), getY()+1);

            return true;
        }
    }
    else if(dir == down){
        if(!getWorld()->actorIsBlockingAtXY(getX(), getY()-1)) {
            setDirection(down);
            moveTo(getX(), getY()-1);

            return true;
        }
    }
    return false;
        
}

void ThiefBot::takeDamage(int damage){
    Alive::takeDamage(damage);
    if(isAlive() == false){
        if(hasGoodie){
            if(hasAmmo)
                getWorld()->addAmmoGoodie(getX(), getY());
            else if(hasLife)
                getWorld()->addLifeGoodie(getX(), getY());
            else if(hasHealth)
                getWorld()->addHealthGoodie(getX(), getY());
        }
    }
}

bool ThiefBot::isThiefBot() const { return true; }


//---------------------------------------------------MEAN THIEFBOTS---------------------------------------------------

MeanThiefBot::MeanThiefBot(StudentWorld* sw, double x, double y) : ThiefBot(sw, x, y, 8, 20, IID_MEAN_THIEFBOT) {}
MeanThiefBot::~MeanThiefBot() {}
void MeanThiefBot::robotDoSomething(){
    if(inRange() && facingPlayer() && !cantShoot())
        shootPea();
    else
        ThiefBot::robotDoSomething();
}
