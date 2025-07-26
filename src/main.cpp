#include <iostream>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <ctime>
#include "../include/World.h"
#include "../include/Entity.h"
#include "../include/Carnivore.h"
#include "../include/Herbivore.h"
#include "../include/Plant.h"
#include "../include/AnimalConfig.h"

using namespace std;

const uint32_t WIDTH = 5;
const uint32_t HEIGHT = 5;

void clearScreen(){
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void sleepMainThread(int milliseconds){
    this_thread::sleep_for(chrono::milliseconds(milliseconds));
}

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

    initializeWorldWithEntities(0, 20, 2);
    int counter = 0;
    while(true){
        world.run();
        world.displayWorld();
        cout << "Iteration: " << counter++ << " - Occupied Cells: " << world.getOccupiedCellsCount() << endl;
        
        if (world.getOccupiedCellsCount() == 0) {
            cout << "WARNING: No entities left in the world!" << endl;
        }
        
        sleepMainThread(800);
        clearScreen();
    }
    return 0;
}