#include <iostream>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <ctime>
#include <SFML/Graphics.hpp>
#include "../include/World.h"
#include "../include/Entity.h"
#include "../include/AnimalConfig.h"

using namespace std;

const uint32_t WIDTH = 15;
const uint32_t HEIGHT = 15;
const uint32_t TILE_SIZE = 48;

void initializeWorldWithEntities(const uint32_t& numPlants, const uint32_t& numHerbivores, const uint32_t& numCarnivores){
    World &world = World::getInstance();
    for(uint32_t i = 0; i < numPlants; ++i){
        world.addEntityType(animalconfig::PLANT_CONFIG.symbol);
    }
    for(uint32_t i = 0; i < numHerbivores; ++i){
        world.addEntityType(animalconfig::HERBIVORE_CONFIG.symbol);
    }
    for(uint32_t i = 0; i < numCarnivores; ++i){
        world.addEntityType(animalconfig::CARNIVORE_CONFIG.symbol);
    }
}
int main(){
    srand(time(0));
    World &world = World::getInstance();
    world.initialize(HEIGHT, WIDTH);

    initializeWorldWithEntities(15, 10, 2);

    sf::RenderWindow window(sf::VideoMode(WIDTH * TILE_SIZE, HEIGHT * TILE_SIZE), "EcoSim");

    std::map<char, sf::Texture> textures;
    if (!textures['H'].loadFromFile("../sprites/sheep.png")) { cerr << "Error loading sheep.png" << endl; return -1; }
    if (!textures['C'].loadFromFile("../sprites/wolf.png")) { cerr << "Error loading wolf.png" << endl; return -1; }
    if (!textures['*'].loadFromFile("../sprites/bush.png")) { cerr << "Error loading bush.png" << endl; return -1; }
    if (!textures['.'].loadFromFile("../sprites/tile.png")) { cerr << "Error loading tile.png" << endl; return -1; }
    textures[' '] = textures['.'];

    int counter = 0;
    uint32_t spawnTime = 0;
    while(window.isOpen()){
        if(spawnTime == 0){
            spawnTime = (rand() % 4) + 4;
        } else if(spawnTime == (counter % 10)){
            world.addEntityType(animalconfig::PLANT_CONFIG.symbol);
            spawnTime = 0;
        }
        sf::Event event;
        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed)
                window.close();
        }

        world.run();
        
        window.clear();
        // Pass the TILE_SIZE to the render function
        world.render(window, textures, TILE_SIZE);
        window.display();

        cout << "Iteration: " << counter++ << " - Occupied Cells: " << world.getOccupiedCellsCount() << endl;
        
        if (world.getOccupiedCellsCount() == 0) {
            cout << "WARNING: No entities left in the world!" << endl;
            break;
        }
        
        this_thread::sleep_for(chrono::milliseconds(400));
    }
    return 0;
}