#include <gtest/gtest.h>
#include "../include/World.h"
#include "../include/Entity.h"
#include "../include/Carnivore.h"
#include "../include/Herbivore.h"
#include <memory>

namespace {
    static uint32_t HEIGHT = 10;
    static uint32_t WIDTH = 10;
}

class WorldTest: public ::testing::Test {
protected:
    void SetUp() override {
        // Create a new test world for each test
        testWorld = World::createTestInstance(WIDTH, HEIGHT);
    }
    
    void TearDown() override {
        // Clean up
        World::resetInstance();
    }

    std::unique_ptr<World> testWorld;
    
public:
    static void runBoundaryTest(const Entity& entity){
        kinematics::Vector2D position = entity.getPosition();

        // Check if the entity is within the world boundaries
        EXPECT_GE(position.x, 0);
        EXPECT_LT(position.x, HEIGHT);
        EXPECT_GE(position.y, 0);
        EXPECT_LT(position.y, WIDTH);
    }
};

TEST_F(WorldTest, WorldInitialization) {
    // Test world is already initialized in SetUp
    EXPECT_EQ(testWorld->getCellId(0, 0), 0);
    EXPECT_EQ(testWorld->getCellId(9, 9), 99);
    EXPECT_EQ(testWorld->getCellId(10, 10), INT_MAX);
}

TEST_F(WorldTest, AddEntity) {
    Entity* entity = new Entity(animalconfig::HERBIVORE_CONFIG, *testWorld, 0, 0);
    testWorld->addEntity(entity);
    
    EXPECT_TRUE(testWorld->isCellOccupied(0, 0));
    EXPECT_EQ(testWorld->getCellCoordinates(testWorld->getCellId(0, 0)), kinematics::Vector2D(0, 0));
    
    // Cleanup
    delete entity;
}

TEST_F(WorldTest, AddEntityType) {
    uint32_t currentOccupiedSize = testWorld->getOccupiedCellsCount();
    testWorld->addEntityType('H');
    EXPECT_EQ(testWorld->getOccupiedCellsCount(), currentOccupiedSize + 1);
}

TEST_F(WorldTest, GetNewEmptyCell) {
    kinematics::Vector2D cell = testWorld->getNewEmptyCell();
    Entity entity(animalconfig::PLANT_CONFIG, *testWorld, cell.x, cell.y);
    testWorld->addEntity(&entity);

    runBoundaryTest(entity);
    EXPECT_TRUE(testWorld->isCellOccupied(cell.x, cell.y));
}

TEST_F(WorldTest, OverFlowAddEntity){
    auto smallWorld = World::createTestInstance(1, WIDTH);

    try{
        for (int i = 0; i < 1; i++){
            for (int j = 0; j <= WIDTH; j++){
                smallWorld->addEntityType('*');
            }
        }

        FAIL() << "Expected to throw error";
    } catch (const std::runtime_error& e) {
        EXPECT_EQ(std::string(e.what()), "World is full");
        SUCCEED() << "Correctly thrown error";
    } catch (...) {
        FAIL() << "Expected std::runtime_error, but got a different exception type.";
    }
}

TEST_F(WorldTest, RunWorld){
    for (int i = 0; i < 5; i++) {
        testWorld->addEntityType('H');
        testWorld->addEntityType('C');
        testWorld->addEntityType('*');
    }
    
    EXPECT_NO_THROW(testWorld->run());
}

TEST_F(WorldTest, GetCellSymbol) {
    Entity* entity = new Entity(animalconfig::HERBIVORE_CONFIG, *testWorld, 0, 0);
    testWorld->addEntity(entity);
    
    EXPECT_EQ(testWorld->getCellSymbol(0, 0), 'H');    
    delete entity;
}

TEST_F(WorldTest, KillEntity){
    Entity* entity = new Entity(animalconfig::HERBIVORE_CONFIG, *testWorld, 0, 0);
    testWorld->addEntity(entity);
    
    EXPECT_TRUE(testWorld->isCellOccupied(0, 0));
    
    testWorld->killEntity(entity);
    
    EXPECT_FALSE(testWorld->isCellOccupied(0, 0));
    EXPECT_EQ(entity, nullptr);
}

TEST_F(WorldTest, GetEntityAt) {
    Entity* entity = new Entity(animalconfig::HERBIVORE_CONFIG, *testWorld, 0, 0);
    testWorld->addEntity(entity);
    
    EXPECT_EQ(testWorld->getEntityAt(0, 0), entity);
    EXPECT_EQ(testWorld->getEntityAt(1, 1), nullptr);
    
    delete entity;
}

TEST_F(WorldTest, GetOccupiedCellsCount) {
    EXPECT_EQ(testWorld->getOccupiedCellsCount(), 0);
    
    testWorld->addEntityType('H');
    EXPECT_EQ(testWorld->getOccupiedCellsCount(), 1);
    
    testWorld->addEntityType('C');
    EXPECT_EQ(testWorld->getOccupiedCellsCount(), 2);
    
    testWorld->addEntityType('*');
    EXPECT_EQ(testWorld->getOccupiedCellsCount(), 3);
}

