#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <unordered_set>
#include <cstdint>
#include <memory>

#include "kinematics.h"
class Entity;

class World {
    private:
    std::vector<std::vector<Entity*>> grid;
    std::unordered_set<uint32_t> gridOccupied;
    static World* instancePtr;
    
    
    public:
        // Protected constructor for testing
        World();
        World(int width, int height);
        kinematics::Vector2D size;
        
        // For production code
        static World& getInstance();
        
        // For testing - creates a new instance each time
        static std::unique_ptr<World> createTestInstance(int width, int height);
        
        // Reset the singleton (for test cleanup)
        static void resetInstance();
        
        World(const World &) = delete;
        World& operator=(const World&) = delete;

        void initialize(int width, int height) {
            size = kinematics::Vector2D(width, height);
            grid.resize(width, std::vector<Entity*>(height, nullptr));
        }

        void addEntity(Entity* entity);
        void addEntityType(char symbol);
        void killEntity(Entity* &entity);
        kinematics::Vector2D getNewEmptyCell();
        void run();

        inline bool isCellOccupied(const int& x, const int& y) const { return gridOccupied.find(getCellId(x, y)) != gridOccupied.end(); }

        inline uint32_t getOccupiedCellsCount() const { return gridOccupied.size(); }
        inline uint32_t getCellId(const int& x, const int& y) const { 
            if(x >= 0 && x < size.x && y >= 0 && y < size.y)
               return x * size.y + y;

            return INT_MAX; //invalid
        }
        inline kinematics::Vector2D getCellCoordinates(const uint32_t& cellId) const {
            return kinematics::Vector2D(cellId / size.y, cellId % size.y);
        }

        char getCellSymbol(const int &x, const int &y) const;
        Entity *getEntityAt(const int &x, const int &y) const;
};
#endif