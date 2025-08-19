#include "../include/World.h"
#include "../include/Entity.h"
#include <climits>
#include <SFML/Graphics.hpp>

// Initialize static member
World* World::instancePtr = nullptr;

// Default constructor
World::World() : size(0, 0) {}

// Constructor implementation
World::World(int width, int height) : size(width, height) {
    if (width > 0 && height > 0) {
        initialize(width, height);
    }
}

// Get singleton instance
World& World::getInstance() {
    if (!instancePtr) {
        instancePtr = new World();
    }
    return *instancePtr;
}

// Create test instance
std::unique_ptr<World> World::createTestInstance(int width, int height) {
    return std::make_unique<World>(width, height);
}

// Reset singleton instance
void World::resetInstance() {
    delete instancePtr;
    instancePtr = nullptr;
}
void World::run(){
    for(int x = 0; x < size.x; ++x){
        for(int y = 0; y < size.y; ++y){
            Entity* entity = grid[x][y];
            if (entity != nullptr) {
                entity->isUpdated = false;
            }
        }
    }
    
    for(int x = 0; x < size.x; ++x){
        for(int y = 0; y < size.y; ++y){
            Entity* entity = grid[x][y];
            if (entity == nullptr || entity->isUpdated) 
                continue;
            if(entity->entityConfig.symbol != animalconfig::HERBIVORE_CONFIG.symbol)
                continue;
            
            if(entity->entityConfig.energy <= 0) {
                killEntity(entity);
                grid[x][y] = nullptr; 
            } else {
                entity->update();
            }

            if(entity != nullptr) {
                entity->isUpdated = true;
            }
        }
    }

    for(int x = 0; x < size.x; ++x){
        for(int y = 0; y < size.y; ++y){
            Entity* entity = grid[x][y];
            if (entity == nullptr || entity->isUpdated) 
                continue;
            
            if(entity->entityConfig.energy <= 0) {
                killEntity(entity);
                grid[x][y] = nullptr; 
            } else {
                entity->update();
            }

            if(entity != nullptr) {
                entity->isUpdated = true;
            }
        }
    }
}
char World::getCellSymbol(const int& x, const int& y) const {
    if (isCellOccupied(x, y)) {
        return grid[x][y]->getSymbol();
    }
    return '.';
}
Entity* World::getEntityAt(const int& x, const int& y) const {
    if (isCellOccupied(x, y)) {
        return grid[x][y];
    }
    return nullptr;
}

void World::addEntity(Entity* entity){
    kinematics::Vector2D pos = entity->getPosition();
    uint32_t cellId = getCellId(pos.x, pos.y);

    grid[pos.x][pos.y] = entity;
    gridOccupied.insert(cellId);
}

void World::addEntityType(char symbol){
    kinematics::Vector2D pos = getNewEmptyCell();
    Entity* newEntity = new Entity(animalconfig::getConfig(symbol), pos.x, pos.y);
    addEntity(newEntity);
}

kinematics::Vector2D World::getNewEmptyCell(){
    uint32_t cellId;
    if (gridOccupied.size() == size.x * size.y) {
        throw std::runtime_error("World is full");
    }
    do{
        cellId = rand() % (size.x * size.y);
    } while (gridOccupied.find(cellId) != gridOccupied.end());

    return kinematics::Vector2D(cellId / size.y, cellId % size.y);
}

void World::killEntity(Entity* &entity) {
    kinematics::Vector2D pos = entity->getPosition();
    uint32_t cellId = getCellId(pos.x, pos.y);

    gridOccupied.erase(cellId);
    grid[pos.x][pos.y] = nullptr;
    delete entity;
    entity = nullptr;
}

void World::displayWorld() const {
    uint32_t numPlants = 0, numHerbivores = 0, numCarnivores = 0;

    for (int x = 0; x < size.x; ++x) {
        for (int y = 0; y < size.y; ++y) {
            char symbol = getCellSymbol(x, y);
            std::cout << symbol << ' ';
            if (symbol == animalconfig::PLANT_CONFIG.symbol) {
                numPlants++;
            } else if (symbol == animalconfig::HERBIVORE_CONFIG.symbol) {
                numHerbivores++;
            } else if (symbol == animalconfig::CARNIVORE_CONFIG.symbol) {
                numCarnivores++;
            }
        }
        std::cout << '\n';
    }
    std::cout << "Plants: " << numPlants << ", Herbivores: " << numHerbivores << ", Carnivores: " << numCarnivores << '\n';
}
void World::render(sf::RenderWindow& window, std::map<char, sf::Texture>& textures, const uint32_t& tileSize) {
    for (int x = 0; x < size.x; ++x) {
        for (int y = 0; y < size.y; ++y) {
            sf::Sprite sprite;
            char symbol = getCellSymbol(x, y);
            
            sf::Texture* texture = &textures[' ']; // Default to tile
            if (textures.count(symbol)) {
                texture = &textures[symbol];
            }
            sprite.setTexture(*texture);

            const Entity* entity = getEntityAt(x, y);
            if(entity != nullptr && (entity->entityConfig.symbol == 'H' || entity->entityConfig.symbol == 'C')) {
                if(entity->entityConfig.energy <= 50) {
                    sf::Color color(255, 100, 100);
                    color.a = 50 + (3 * entity->entityConfig.energy);
                    sprite.setColor(color);
                }
            }

            // color.a = 200;
            // sprite.setColor(color);
            // --- SCALING LOGIC ---
            // Get the original size of the texture
            sf::Vector2u textureSize = texture->getSize();
            // Calculate the scale factor needed to fit the tile size
            float scaleX = (float)tileSize / textureSize.x;
            float scaleY = (float)tileSize / textureSize.y;
            // Apply the scale
            sprite.setScale(scaleX, scaleY);
            
            sprite.setPosition(y * tileSize, x * tileSize);
            window.draw(sprite);
        }
    }
}