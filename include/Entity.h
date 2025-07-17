#ifndef ENTITY_H
#define ENTITY_H

#include "Kinematics.h"

class Entity: public kinematics::Body {
    public:
        char symbol;

        Entity(char symbol, int posX, int posY, int velX, int velY)
            : symbol(symbol), kinematics::Body(posX, posY, velX, velY)
        {}

        Entity(char symbol, int posX, int posY)
            : symbol(symbol), kinematics::Body(posX, posY)
        {}

        void update(){}
};

#endif