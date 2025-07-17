#ifndef Herbivore_H
#define Herbivore_H

#include "Entity.h"

class Herbivore: public Entity {
    public:
        Herbivore(char symbol = 'H', int posX, int posY, int velX, int velY)
            : Entity(symbol, posX, posY, velX, velY)
        {}

        Herbivore(char symbol = 'H', int posX, int posY)
            : Entity(symbol, posX, posY)
        {}
};

#endif