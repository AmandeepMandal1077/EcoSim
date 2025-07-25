#include <gtest/gtest.h>
#include "../include/AnimalConfig.h"

TEST(AnimalConfigTest, GetEntityRank) {
    EXPECT_EQ(animalconfig::getEntityRank('*'), 1);
    EXPECT_EQ(animalconfig::getEntityRank('H'), 2);
    EXPECT_EQ(animalconfig::getEntityRank('C'), 3);
    EXPECT_EQ(animalconfig::getEntityRank('X'), 0); // Invalid symbol
}

TEST(AnimalConfigTest, HerbivoreConfig) {
    EXPECT_EQ(animalconfig::HERBIVORE_CONFIG.symbol, 'H');
    EXPECT_EQ(animalconfig::HERBIVORE_CONFIG.prey, '*');
    EXPECT_EQ(animalconfig::HERBIVORE_CONFIG.energy, 100);
    EXPECT_EQ(animalconfig::HERBIVORE_CONFIG.maxEnergy, 200);
    EXPECT_EQ(animalconfig::HERBIVORE_CONFIG.visionRange, 5);
    EXPECT_EQ(animalconfig::HERBIVORE_CONFIG.reproductionCost, 50);
    EXPECT_EQ(animalconfig::HERBIVORE_CONFIG.reproductionThreshold, 150);
    EXPECT_EQ(animalconfig::HERBIVORE_CONFIG.energyGainFromEating, 20);
    EXPECT_EQ(animalconfig::HERBIVORE_CONFIG.energyCostPerTick, 1);
}

