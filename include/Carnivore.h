#ifndef CARNIVORE_H
#define CARNIVORE_H

#include "Entity.h"

class Carnivore: public Entity {
    public:
        Carnivore(char symbol = 'C', int posX, int posY, int velX, int velY)
            : Entity(symbol, posX, posY, velX, velY)
        {}

        Carnivore(char symbol = 'C', int posX, int posY)
            : Entity(symbol, posX, posY)
        {}
};

#endif