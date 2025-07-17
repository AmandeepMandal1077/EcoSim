#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <unordered_set>
#include <cstdint>

#include "Entity.h"
#include "kinematics.h"

class World {
    private:
        std::vector<Entity> entities;
        kinematics::Vector2D size;
        std::vector<std::vector<Entity>> grid;
        std::unordered_set<uint32_t> gridOccupied;

    public:
        World(int width, int height)
            : size(width, height), grid(width, std::vector<Entity>(height)) {}

        void addEntity(const Entity &entity);
        void update();
        kinematics::Vector2D getNewEmptyCell();

        inline uint32_t getCellId(const int& x, const int& y) const { return x * size.y + y; }
        inline kinematics::Vector2D getCellCoordinates(const uint32_t& cellId) const {
            return kinematics::Vector2D(cellId / size.y, cellId % size.y);
        }
};

void World::update(){
    for(auto& entity: entities){
        entity.update();
    }
}

void World::addEntity(const Entity& entity){
    entities.push_back(entity);

    kinematics::Vector2D pos = entity.getPosition();
    uint32_t cellId = getCellId(pos.x, pos.y);

    grid[pos.x][pos.y] = entity;
    gridOccupied.insert(cellId);
}


kinematics::Vector2D World::getNewEmptyCell(){
    uint32_t cellId;
    do{
        cellId = rand() % (size.x * size.y);
    } while (gridOccupied.find(cellId) != gridOccupied.end());

    return kinematics::Vector2D(cellId / size.y, cellId % size.y);
}

#endif