#ifndef Herbivore_H
#define Herbivore_H

#include "Entity.h"

class Herbivore: public Entity {
    public:

        Herbivore(int posX, int posY, int velX, int velY)
            : Entity(animalconfig::HERBIVORE_CONFIG, posX, posY, velX, velY)
        {}

        Herbivore(int posX, int posY)
            : Entity(animalconfig::HERBIVORE_CONFIG, posX, posY)
        {}

        virtual void update() override;
};

#endif