#ifndef ENTITY_H
#define ENTITY_H

#include "Kinematics.h"
#include "AnimalConfig.h"

class World;

class Entity: public kinematics::Body {
    protected:
        World &world;

    public:
        animalconfig::config entityConfig;
        
        // For production code (uses singleton)
        Entity(animalconfig::config config, int posX, int posY, int velX, int velY);
        Entity(animalconfig::config config, int posX, int posY);
        Entity(animalconfig::config config, kinematics::Vector2D pos);
        
        // For testing (accepts any world)
        Entity(animalconfig::config config, World& testWorld, int posX, int posY, int velX, int velY);
        Entity(animalconfig::config config, World& testWorld, int posX, int posY);
        Entity(animalconfig::config config, World& testWorld, kinematics::Vector2D pos);

        virtual void update();

        void moveRandom();
        void moveAwayFromEntity(Entity* entity);
        void moveTowardsEntity(Entity* entity);
        void moveTowardsPosition(int posX, int posY);
        void feed(Entity* &prey);
        bool reproduce();
        kinematics::Vector2D findNearestPredator();
        kinematics::Vector2D findNearestPrey();

        char getSymbol() const { return entityConfig.symbol; }  

        inline void tickEnergy() { entityConfig.energy -= entityConfig.energyCostPerTick; }
        bool checkBound(kinematics::Vector2D& pos);
};

#endif