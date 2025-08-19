#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <unordered_set>
#include <cstdint>
#include <memory>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <map>

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
            grid.resize(height, std::vector<Entity*>(width, nullptr));
        }

        void addEntity(Entity* entity);
        void addEntityType(char symbol);
        void killEntity(Entity* &entity);
        kinematics::Vector2D getNewEmptyCell();
        void run();
        // Updated render function signature
        void render(sf::RenderWindow& window, std::map<char, sf::Texture>& textures, const uint32_t& tileSize);

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
        void displayWorld() const;
        void setEntityAt(const int &x, const int &y, Entity* entity) {
            if (x >= 0 && x < size.x && y >= 0 && y < size.y) {
                grid[x][y] = entity;
                gridOccupied.insert(getCellId(x, y));
            } else {
                std::cerr << "ERROR: Attempt to set entity at invalid position: " << x << ", " << y << std::endl;
            }
        }
        void clearCell(const int &x, const int &y) {
            if (x >= 0 && x < size.x && y >= 0 && y < size.y) {
                grid[x][y] = nullptr;
                uint32_t cellId = getCellId(x, y);
                if (gridOccupied.find(cellId) != gridOccupied.end()) {
                    gridOccupied.erase(cellId);
                }
            } else {
                std::cerr << "ERROR: Attempt to clear invalid cell: " << x << ", " << y << std::endl;
            }
        }
};
#endif