#ifndef CARNIVORE_H
#define CARNIVORE_H

#include "Entity.h"

class Carnivore: public Entity {
    public:
        Carnivore(int posX, int posY, int velX, int velY)
            : Entity(animalconfig::CARNIVORE_CONFIG, posX, posY, velX, velY)
        {}

        Carnivore(int posX, int posY)
            : Entity(animalconfig::CARNIVORE_CONFIG, posX, posY)
        {}

        virtual void update() override;
};

#endif