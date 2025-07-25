#include <gtest/gtest.h>
#include "../include/Entity.h"
#include "../include/World.h"
#include "../include/Kinematics.h"
#include "../include/AnimalConfig.h"
#include <memory>

namespace {
    static uint32_t HEIGHT = 10;
    static uint32_t WIDTH = 10;
}
inline uint32_t distance(const kinematics::Vector2D& pos1, const kinematics::Vector2D& pos2) {
    return abs(pos1.x - pos2.x) + abs(pos1.y - pos2.y);
}

class EntityTest: public ::testing::Test {
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

    static void runBoundaryTest(const Entity* entity){
        kinematics::Vector2D position = entity->getPosition();

        // Check if the entity is within the world boundaries
        EXPECT_GE(position.x, 0);
        EXPECT_LT(position.x, HEIGHT);
        EXPECT_GE(position.y, 0);
        EXPECT_LT(position.y, WIDTH);
    }
};

TEST_F(EntityTest, EntityInitialization) {
    Entity entity(animalconfig::HERBIVORE_CONFIG, *testWorld, 0, 0, 1, 1);
    EXPECT_EQ(entity.entityConfig.symbol, 'H');
    EXPECT_EQ(entity.getPosition().x, 0);
    EXPECT_EQ(entity.getPosition().y, 0);
    EXPECT_EQ(entity.getVelocity().x, 1);
    EXPECT_EQ(entity.getVelocity().y, 1);
}

TEST_F(EntityTest, EntityMoveTowardsPosition) {
    kinematics::Vector2D targetPosition(5, 5);
    uint32_t initialDistance = distance(kinematics::Vector2D(0, 0), targetPosition);

    Entity* entity = (new Entity(animalconfig::HERBIVORE_CONFIG, *testWorld, 0, 0));

    entity->moveTowardsPosition(targetPosition.x, targetPosition.y);
    entity->applyVelocity();
    uint32_t newDistance = distance(entity->getPosition(), targetPosition);

    EXPECT_LT(newDistance, initialDistance);
    runBoundaryTest(*entity);

    delete entity;
}

TEST_F(EntityTest, EntityMoveTowardsEntity) {
    Entity entity1(animalconfig::CARNIVORE_CONFIG, *testWorld, 0, 0);
    Entity entity2(animalconfig::HERBIVORE_CONFIG, *testWorld, 5, 5);

    uint32_t initialDistance = distance(entity1.getPosition(), entity2.getPosition());

    entity1.moveTowardsEntity(&entity2);
    entity1.applyVelocity();
    uint32_t newDistance = distance(entity1.getPosition(), entity2.getPosition());

    EXPECT_LT(newDistance, initialDistance);
    runBoundaryTest(entity1);
}

TEST_F(EntityTest, NOEntityMoveTowardsEntity) {
    Entity entity1(animalconfig::HERBIVORE_CONFIG, *testWorld, 2, 2);
    Entity entity2(animalconfig::HERBIVORE_CONFIG, *testWorld, 0, 0);

    Entity entity3(animalconfig::HERBIVORE_CONFIG, *testWorld, 2, 1);
    Entity entity4(animalconfig::HERBIVORE_CONFIG, *testWorld, 2, 3);
    Entity entity5(animalconfig::HERBIVORE_CONFIG, *testWorld, 1, 2);
    Entity entity6(animalconfig::HERBIVORE_CONFIG, *testWorld, 3, 2);

    testWorld->addEntity(&entity1);
    testWorld->addEntity(&entity2);
    testWorld->addEntity(&entity3);
    testWorld->addEntity(&entity4);
    testWorld->addEntity(&entity5);
    testWorld->addEntity(&entity6);

    uint32_t initialDistance = distance(entity1.getPosition(), entity2.getPosition());

    entity1.moveTowardsEntity(&entity2);
    entity1.applyVelocity();
    uint32_t newDistance = distance(entity1.getPosition(), entity2.getPosition());

    EXPECT_EQ(newDistance, initialDistance);
    EXPECT_EQ(entity1.getPosition(), kinematics::Vector2D(2, 2));
    runBoundaryTest(entity1);
}

TEST_F(EntityTest, EntityMoveAwayFromEntity) {
    Entity entity1(animalconfig::HERBIVORE_CONFIG, *testWorld, 1, 1);
    Entity entity2(animalconfig::CARNIVORE_CONFIG, *testWorld, 5, 5);
    
    uint32_t initialDistance = distance(entity1.getPosition(), entity2.getPosition());
    
    entity1.moveAwayFromEntity(&entity2);
    entity1.applyVelocity();
    uint32_t newDistance = distance(entity1.getPosition(), entity2.getPosition());
    
    EXPECT_GT(newDistance, initialDistance);
    runBoundaryTest(entity1);
}

TEST_F(EntityTest, NOEntityMoveAwayEntity) {
    Entity entity1(animalconfig::HERBIVORE_CONFIG, *testWorld, 2, 2);
    Entity entity2(animalconfig::HERBIVORE_CONFIG, *testWorld, 0, 0);
    Entity entity3(animalconfig::HERBIVORE_CONFIG, *testWorld, 2, 1);
    Entity entity4(animalconfig::HERBIVORE_CONFIG, *testWorld, 2, 3);
    Entity entity5(animalconfig::HERBIVORE_CONFIG, *testWorld, 1, 2);
    Entity entity6(animalconfig::HERBIVORE_CONFIG, *testWorld, 3, 2);

    testWorld->addEntity(&entity1);
    testWorld->addEntity(&entity2);
    testWorld->addEntity(&entity3);
    testWorld->addEntity(&entity4);
    testWorld->addEntity(&entity5);
    testWorld->addEntity(&entity6);

    uint32_t initialDistance = distance(entity1.getPosition(), entity2.getPosition());

    entity1.moveAwayFromEntity(&entity2);
    entity1.applyVelocity();
    uint32_t newDistance = distance(entity1.getPosition(), entity2.getPosition());

    EXPECT_EQ(newDistance, initialDistance);
    EXPECT_EQ(entity1.getPosition(), kinematics::Vector2D(2, 2));
    runBoundaryTest(entity1);
}
TEST_F(EntityTest, EntityRandomMovement) {
    Entity entity(animalconfig::HERBIVORE_CONFIG, *testWorld, 0, 0);
    kinematics::Vector2D initialPosition = entity.getPosition();

    testWorld->addEntity(&entity);

    entity.moveRandom();
    entity.applyVelocity();
    kinematics::Vector2D newPosition = entity.getPosition();

    EXPECT_NE(newPosition, initialPosition);
    runBoundaryTest(entity);
}
TEST_F(EntityTest, NOEntityRandomMovement) {
    Entity entity(animalconfig::HERBIVORE_CONFIG, *testWorld, 1, 1);
    Entity entity1(animalconfig::HERBIVORE_CONFIG, *testWorld, 0, 1);
    Entity entity2(animalconfig::HERBIVORE_CONFIG, *testWorld, 2, 1);
    Entity entity3(animalconfig::HERBIVORE_CONFIG, *testWorld, 1, 0);
    Entity entity4(animalconfig::HERBIVORE_CONFIG, *testWorld, 1, 2);
    kinematics::Vector2D initialPosition = entity.getPosition();

    testWorld->addEntity(&entity);
    testWorld->addEntity(&entity1);
    testWorld->addEntity(&entity2);
    testWorld->addEntity(&entity3);
    testWorld->addEntity(&entity4);

    entity.moveRandom();
    entity.applyVelocity();
    kinematics::Vector2D newPosition = entity.getPosition();

    EXPECT_EQ(newPosition, initialPosition);
    runBoundaryTest(entity);
}

TEST_F(EntityTest, EntityFeeding) {
    Entity* predator = new Entity(animalconfig::CARNIVORE_CONFIG, *testWorld, 1, 0);
    Entity* prey = new Entity(animalconfig::HERBIVORE_CONFIG, *testWorld, 0, 0);

    testWorld->addEntity(predator);
    testWorld->addEntity(prey);
    uint32_t initialEnergy = predator->entityConfig.energy;
    predator->feed(prey);

    EXPECT_GT(predator->entityConfig.energy, initialEnergy);
    runBoundaryTest(*predator);

    delete predator;
    if(prey){
        delete prey;
    }
}
TEST_F(EntityTest, CannotEntityFeeding) {
    Entity* predator = new Entity(animalconfig::CARNIVORE_CONFIG, *testWorld, 1, 1);
    Entity* prey = new Entity(animalconfig::HERBIVORE_CONFIG, *testWorld, 0, 0);

    testWorld->addEntity(predator);
    testWorld->addEntity(prey);
    uint32_t initialEnergy = predator->entityConfig.energy;
    predator->feed(prey);

    EXPECT_EQ(predator->entityConfig.energy, initialEnergy);
    runBoundaryTest(*predator);

    delete predator;
    delete prey;
}

TEST_F(EntityTest, NOEntityFeeding) {
    try{
        Entity* predator = new Entity(animalconfig::CARNIVORE_CONFIG, *testWorld, 0, 0);
        Entity* prey = new Entity(animalconfig::HERBIVORE_CONFIG, *testWorld, 0, 0);

        testWorld->addEntity(predator);
        testWorld->addEntity(prey);
        uint32_t initialEnergy = predator->entityConfig.energy;
        predator->feed(prey);

        delete predator;
        if(prey != nullptr) {
            delete prey;
        }
        FAIL() << "EXPECTED ERROR";
    } catch (const std::runtime_error& e){
        EXPECT_EQ(std::string(e.what()), "Two Entities at same pos");
        SUCCEED() << "Successfully catch error";
    } catch (...){
        FAIL() << "Failed to catch error";
    }
}

TEST_F(EntityTest, EntityReproduction) {
    Entity entity(animalconfig::HERBIVORE_CONFIG, *testWorld, 0, 0);
    entity.entityConfig.energy = entity.entityConfig.reproductionThreshold;
    testWorld->addEntity(&entity);

    bool reproduced = entity.reproduce();
    EXPECT_TRUE(reproduced);
    runBoundaryTest(entity);
}

TEST_F(EntityTest, EntityFindNearestPrey){
    Entity herbivore(animalconfig::HERBIVORE_CONFIG, *testWorld, 0, 0);
    Entity plant(animalconfig::PLANT_CONFIG, *testWorld, 1, 1);
    testWorld->addEntity(&herbivore);
    testWorld->addEntity(&plant);

    kinematics::Vector2D nearestPrey = herbivore.findNearestPrey();
    EXPECT_EQ(nearestPrey.x, 1);
    EXPECT_EQ(nearestPrey.y, 1);
    runBoundaryTest(herbivore);
}

TEST_F(EntityTest, EntityFindNearestPredator) {
    Entity carnivore(animalconfig::CARNIVORE_CONFIG, *testWorld, 0, 0);
    Entity herbivore(animalconfig::HERBIVORE_CONFIG, *testWorld, 1, 1);
    testWorld->addEntity(&carnivore);
    testWorld->addEntity(&herbivore);

    kinematics::Vector2D nearestPredator = herbivore.findNearestPredator();
    EXPECT_EQ(nearestPredator.x, 0);
    EXPECT_EQ(nearestPredator.y, 0);
    runBoundaryTest(herbivore);
}

TEST_F(EntityTest, EntityTickEnergy) {
    Entity entity(animalconfig::HERBIVORE_CONFIG, *testWorld, 0, 0);
    uint32_t initialEnergy = entity.entityConfig.energy;

    testWorld->addEntity(&entity);

    entity.tickEnergy();
    EXPECT_LT(entity.entityConfig.energy, initialEnergy);
    runBoundaryTest(entity);
}