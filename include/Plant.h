#ifndef PLANT_H
#define PLANT_H

#include "Entity.h"

class Plant: public Entity {
    public:
        Plant(int posX, int posY)
            : Entity(animalconfig::PLANT_CONFIG, posX, posY)
        {}

        virtual void update() override;
};

#endif