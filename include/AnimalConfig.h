#ifndef ANIMALCONFIG_H
#define ANIMALCONFIG_H

#include <cstdint>

namespace animalconfig{
    struct config {
        char symbol;
        char prey;
        uint32_t energy;
        uint32_t maxEnergy;
        uint32_t visionRange;
        uint32_t reproductionCost;
        uint32_t reproductionThreshold;
        uint32_t energyGainFromEating;
        uint32_t energyCostPerTick;
    };

    inline uint8_t getEntityRank(const char& symbol){
        switch (symbol) {
            case '*':
                return 1;
            case 'H':
                return 2;
            case 'C':
                return 3;
            default:
                return 0;
        }
    }

    
    const config HERBIVORE_CONFIG = {
        symbol: 'H',
        prey: '*',
        energy: 100,
        maxEnergy: 200,
        visionRange: 2,
        reproductionCost: 100,
        reproductionThreshold: 150,
        energyGainFromEating: 20,
        energyCostPerTick: 1
    };
    
    const config CARNIVORE_CONFIG = {
        symbol : 'C',
        prey : 'H',
        energy : 120,
        maxEnergy : 220,
        visionRange : 5,
        reproductionCost : 120,
        reproductionThreshold : 180,
        energyGainFromEating : 30,
        energyCostPerTick : 2
    };
    
    const config PLANT_CONFIG = {
        symbol: '*',
        prey: ' ',
        energy: 1,
        maxEnergy: 0,
        visionRange: 0,
        reproductionCost: 0,
        reproductionThreshold: 0,
        energyGainFromEating: 0,
        energyCostPerTick: 0
    };

    inline config getConfig(char symbol) {
        switch (symbol) {
            case 'H':
                return HERBIVORE_CONFIG;
            case 'C':
                return CARNIVORE_CONFIG;
            case '*':
                return PLANT_CONFIG;
            default:
                return PLANT_CONFIG;
        }
    }
}

#endif