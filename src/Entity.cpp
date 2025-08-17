#include "../include/Entity.h"
#include "../include/World.h"
#include <set>
#include <utility>
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

// Standard constructors using singleton
Entity::Entity(animalconfig::config config, int posX, int posY, int velX, int velY)
    : entityConfig(config), kinematics::Body(posX, posY, velX, velY), world(World::getInstance())
{}

Entity::Entity(animalconfig::config config, int posX, int posY)
    : entityConfig(config), kinematics::Body(posX, posY), world(World::getInstance())
{}

Entity::Entity(animalconfig::config config, kinematics::Vector2D pos)
    : entityConfig(config), kinematics::Body(pos), world(World::getInstance())
{}

// Test-specific constructors with dependency injection
Entity::Entity(animalconfig::config config, World& testWorld, int posX, int posY, int velX, int velY)
    : entityConfig(config), kinematics::Body(posX, posY, velX, velY), world(testWorld)
{}

Entity::Entity(animalconfig::config config, World& testWorld, int posX, int posY)
    : entityConfig(config), kinematics::Body(posX, posY), world(testWorld)
{}

Entity::Entity(animalconfig::config config, World& testWorld, kinematics::Vector2D pos)
    : entityConfig(config), kinematics::Body(pos), world(testWorld)
{}

bool Entity::checkBound(kinematics::Vector2D& pos){
    return pos.x >= 0 && pos.x < world.size.x && pos.y >= 0 && pos.y < world.size.y;
}

inline uint32_t findDistance(const kinematics::Vector2D& pos1, const kinematics::Vector2D& pos2) {
    return abs(pos1.x - pos2.x) + abs(pos1.y - pos2.y);
}
// Key fixes for Entity.cpp update() method:

void Entity::update(){
    if(entityConfig.symbol != animalconfig::CARNIVORE_CONFIG.symbol && 
       entityConfig.symbol != animalconfig::HERBIVORE_CONFIG.symbol) {
        return;
    }

    kinematics::Vector2D currentPos = getPosition();
    kinematics::Vector2D predatorPos = findNearestPredator();

    if(predatorPos.x != -1 && predatorPos.y != -1) {
        moveAwayFromEntity(world.getEntityAt(predatorPos.x, predatorPos.y));
        
        world.clearCell(currentPos.x, currentPos.y);
        float probabilityToMove = static_cast<float>(rand()) / RAND_MAX;
        if(probabilityToMove >= 0.3){
            applyVelocity();
            tickEnergy();
        }

        world.setEntityAt(getPosition().x, getPosition().y, this);
        
        // std::cout << "Fleeing from predator" << std::endl;
        return;
    }

    if(reproduce()){
        return;
    }

    kinematics::Vector2D preyPos = findNearestPrey();
    if(preyPos.x != -1 && preyPos.y != -1){
        Entity* prey = world.getEntityAt(preyPos.x, preyPos.y);
        
        if(findDistance(currentPos, preyPos) == 1) {
            feed(prey);
        }
        
        moveTowardsPosition(preyPos.x, preyPos.y);
        
        world.clearCell(currentPos.x, currentPos.y);
        applyVelocity();
        world.setEntityAt(getPosition().x, getPosition().y, this);

        // std::cout << "Hunting prey" << std::endl;
        return;
    }

    moveRandom();
    
    world.clearCell(currentPos.x, currentPos.y);
    applyVelocity();
    world.setEntityAt(getPosition().x, getPosition().y, this);
    // std::cout << "en: " << entityConfig.energy << " Random movement: " << getVelocity().x << ", " << getVelocity().y << std::endl;
}

void Entity::moveRandom(){
    if(entityConfig.energy <= 0 || world.getOccupiedCellsCount() == world.size.x * world.size.y) {
        setVelocity(0, 0);
        return;
    }

    std::set<std::pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    while(directions.size() > 0){
        int rndIdx = rand() % directions.size();
        auto it = directions.begin();
        advance(it, rndIdx);

        kinematics::Vector2D newPos = getPosition() + kinematics::Vector2D(it->first, it->second);

        if(!checkBound(newPos)) {
            directions.erase(it);
            continue;
        }
        
        char newCellSymbol = world.getCellSymbol(newPos.x, newPos.y);
        if(newCellSymbol != '.'){
            directions.erase(it);
            continue;
        }

        setVelocity(it->first, it->second);
        tickEnergy();
        return;
    }

    setVelocity(0, 0);
}

void Entity::moveAwayFromEntity(Entity* entity){
    if(entity == nullptr || entityConfig.energy <= 0) {
        setVelocity(0, 0);
        return;
    }
    kinematics::Vector2D predatorPos = entity->getPosition();
    kinematics::Vector2D preyPos = getPosition();

    uint8_t preyRank = animalconfig::getEntityRank(entityConfig.symbol);
    uint32_t curDist = findDistance(predatorPos, preyPos);

    for (int x = -1; x <= 1; x++){
        for (int y = -1; y <= 1; y++){
            if(abs(x) == abs(y))
                continue;

            kinematics::Vector2D newPos = preyPos + kinematics::Vector2D(x, y);
            uint32_t newDist = findDistance(predatorPos, newPos);

            if(!checkBound(newPos))
                continue;

            char newCellSymbol = world.getCellSymbol(newPos.x, newPos.y);
            if(preyRank <= animalconfig::getEntityRank(newCellSymbol))
                continue;

            if(newDist > curDist){
                setVelocity(x, y);
                return;
            }
        }
    }

    setVelocity(0, 0);
}

void Entity::moveTowardsEntity(Entity* entity){
    if(entityConfig.energy <= 0) {
        setVelocity(0, 0);
        return;
    }

    kinematics::Vector2D targetPos = entity->getPosition();
    moveTowardsPosition(targetPos.x, targetPos.y);
}


void Entity::moveTowardsPosition(int posX, int posY){
    kinematics::Vector2D targetPos(posX, posY);
    kinematics::Vector2D currentPos = getPosition();

    uint32_t curDist = findDistance(targetPos, currentPos);
    uint8_t currentRank = animalconfig::getEntityRank(entityConfig.symbol);

    for (int x = -1; x <= 1; x++){
        for(int y = -1; y <= 1; y++){
            if(abs(x) == abs(y))
            continue;

            kinematics::Vector2D newPos = currentPos + kinematics::Vector2D(x, y);
            uint32_t newDist = findDistance(targetPos, newPos);

            if(!checkBound(newPos))
                continue;
                
            char newCellSymbol = world.getCellSymbol(newPos.x, newPos.y);
            if(currentRank <= animalconfig::getEntityRank(newCellSymbol))
                continue;

            if(newDist < curDist){
                setVelocity(x, y);
                tickEnergy();
                return;
            }
        }
    }
    
    setVelocity(0, 0);
}

void Entity::feed(Entity* &prey){
    if(prey == nullptr){
        return;
    }
    kinematics::Vector2D preyPos = prey->getPosition();
    kinematics::Vector2D predatorPos = getPosition();

    if(preyPos == predatorPos){
        throw std::runtime_error("Two Entities at same pos");
    }
    if(findDistance(preyPos, predatorPos) > 1){
        return;
    }

    char preySymbol = world.getCellSymbol(preyPos.x, preyPos.y);
    if(preySymbol == '.')
        return;

    char predatorSymbol = world.getCellSymbol(predatorPos.x, predatorPos.y);

    if((preySymbol == animalconfig::PLANT_CONFIG.symbol && predatorSymbol == animalconfig::CARNIVORE_CONFIG.symbol) || preySymbol == predatorSymbol){
        return;
    }

    entityConfig.energy += entityConfig.energyGainFromEating;
    entityConfig.energy = std::min(entityConfig.energy, entityConfig.maxEnergy);
    world.killEntity(prey);
}

bool Entity::reproduce(){
    animalconfig::config &config = entityConfig;
    
    if(config.energy < config.reproductionThreshold){
        return false;
    }
    
    kinematics::Vector2D directions[4] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    
    kinematics::Vector2D reproductionPos = {-1, -1};
    for(auto& dir: directions){
        kinematics::Vector2D newPos = getPosition() + dir;
        if(newPos.x >= 0 && newPos.x < world.size.x && newPos.y >= 0 && newPos.y < world.size.y){
            reproductionPos = newPos;
            break;
        }
    }

    if(reproductionPos == kinematics::Vector2D{-1, -1}){
        return false;
    }
    
    entityConfig.energy -= config.reproductionCost;
    Entity *child = new Entity(animalconfig::getConfig(config.symbol), reproductionPos);
    world.addEntity(child);
    return true;
}

kinematics::Vector2D Entity::findNearestPrey(){
    kinematics::Vector2D currentPos = getPosition();

    int minRow = std::max(0, (int)(currentPos.x - entityConfig.visionRange));
    int maxRow = std::min(world.size.x - 1, (int)(currentPos.x + entityConfig.visionRange));

    int minCol = std::max(0, (int)(currentPos.y - entityConfig.visionRange));
    int maxCol = std::min(world.size.y - 1, (int)(currentPos.y + entityConfig.visionRange));

    int preyX(-1), preyY(-1);
    uint32_t minDist = INT_MAX;

    for(int row = minRow; row <= maxRow; ++row){
        for(int col = minCol; col <= maxCol; ++col){
            if(entityConfig.prey == world.getCellSymbol(row, col)){
                uint32_t currentDist = findDistance(currentPos, kinematics::Vector2D(row, col));
                if(currentDist < minDist){
                    minDist = currentDist;
                    preyX = row;
                    preyY = col;
                }
            }
        }
    }

    return kinematics::Vector2D(preyX, preyY);
}

kinematics::Vector2D Entity::findNearestPredator(){
    kinematics::Vector2D currentPos = getPosition();
    if(entityConfig.energy <= 0) {
        return kinematics::Vector2D(-1, -1);
    }

    uint8_t preyRank = animalconfig::getEntityRank(entityConfig.symbol);
    
    int minRow = std::max(0, (int)(currentPos.x - entityConfig.visionRange));
    int maxRow = std::min(world.size.x - 1, (int)(currentPos.x + entityConfig.visionRange));
    int minCol = std::max(0, (int)(currentPos.y - entityConfig.visionRange));
    int maxCol = std::min(world.size.y - 1, (int)(currentPos.y + entityConfig.visionRange));

    kinematics::Vector2D predatorPos(-1, -1);
    uint32_t minDist = INT_MAX;

    for(int row = minRow; row <= maxRow; ++row){
        for(int col = minCol; col <= maxCol; ++col){
            uint8_t cellEntityRank = animalconfig::getEntityRank(world.getCellSymbol(row, col));

            if(cellEntityRank > preyRank) {
                uint32_t currentDist = findDistance(currentPos, kinematics::Vector2D(row, col));
                if(currentDist < minDist){
                    minDist = currentDist;
                    predatorPos = kinematics::Vector2D(row, col);
                }
            }
        }
    }
    return predatorPos;
}