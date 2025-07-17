#ifndef PLANT_H
#define PLANT_H

#include "Entity.h"

class Plant: public Entity {
    public:
        Plant(char symbol = '*', int posX, int posY)
            : Entity(symbol, posX, posY)
        {}
};

#endif