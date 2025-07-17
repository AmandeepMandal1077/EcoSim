#include <gtest/gtest.h>
#include "../include/Kinematics.h"

TEST(Vector2DTest, Equality){
    kinematics::Vector2D vec1(3, 4);
    kinematics::Vector2D vec2(3, 4);
    
    EXPECT_TRUE(vec1 == vec2);
    EXPECT_FALSE(vec1 == kinematics::Vector2D(5, 6));
}

TEST(Vector2DTest, Inequality){
    kinematics::Vector2D vec1(3, 4);
    kinematics::Vector2D vec2(5, 6);
    
    EXPECT_TRUE(vec1 != vec2);
    EXPECT_FALSE(vec1 != kinematics::Vector2D(3, 4));
}

TEST(Vector2DTest, Addition){
    kinematics::Vector2D vec1(3, 4);
    kinematics::Vector2D vec2(1, 2);
    kinematics::Vector2D result = vec1 + vec2;
    
    EXPECT_EQ(result, kinematics::Vector2D(4, 6));
    EXPECT_TRUE(result == kinematics::Vector2D(4, 6));
}

TEST(Vector2DTest, InPlaceAddition){
    kinematics::Vector2D vec1(1, 2);
    kinematics::Vector2D vec2(3, 4);
    
    vec1 += vec2;
    
    EXPECT_EQ(vec1, kinematics::Vector2D(4, 6));
    EXPECT_TRUE(vec1 == kinematics::Vector2D(4, 6));
}

TEST(Vector2DTest, Subtraction){
    kinematics::Vector2D vec1(5, 7);
    kinematics::Vector2D vec2(2, 3);
    kinematics::Vector2D result = vec1 - vec2;
    
    EXPECT_EQ(result, kinematics::Vector2D(3, 4));
    EXPECT_TRUE(result == kinematics::Vector2D(3, 4));
}

TEST(Vector2DTest, InPlaceSubtraction){
    kinematics::Vector2D vec1(5, 7);
    kinematics::Vector2D vec2(2, 3);
    
    vec1 -= vec2;
    
    EXPECT_EQ(vec1, kinematics::Vector2D(3, 4));
    EXPECT_TRUE(vec1 == kinematics::Vector2D(3, 4));
}

TEST(Vector2DTest, ScalarMultiplication){
    kinematics::Vector2D vec1(2, 3);
    const int scalar = 4;

    kinematics::Vector2D result = vec1 * scalar;

    EXPECT_EQ(result, kinematics::Vector2D(8, 12));
    EXPECT_TRUE(result == kinematics::Vector2D(8, 12));
}

TEST(Vector2DTest, InPlaceScalarMultiplication){
    kinematics::Vector2D vec1(2, 3);
    const int scalar = 5;

    vec1 *= scalar;

    EXPECT_EQ(vec1, kinematics::Vector2D(10, 15));
    EXPECT_TRUE(vec1 == kinematics::Vector2D(10, 15));
}

TEST(StateTest, SetVelocity){
    kinematics::State state(0, 0, 1, 1);
    
    state.setVelocity(2, 3);
    
    EXPECT_EQ(state.getVelocity(), kinematics::Vector2D(2, 3));
    EXPECT_TRUE(state.getVelocity() == kinematics::Vector2D(2, 3));
}

TEST(StateTest, ApplyVelocityToPosition){
    kinematics::State state(2, 3, 1, 1);

    state.applyVelocity(3, -4);
    EXPECT_EQ(state.getPosition(), kinematics::Vector2D(5, -1));
    EXPECT_TRUE(state.getPosition() == kinematics::Vector2D(5, -1));
}

TEST(StateTest, ApplyVelocityWithVector){
    kinematics::State state(1, 1, 0, 0);
    kinematics::Vector2D velocity(2, 3);

    state.applyVelocity(velocity);
    EXPECT_EQ(state.getPosition(), kinematics::Vector2D(3, 4));
    EXPECT_TRUE(state.getPosition() == kinematics::Vector2D(3, 4));
}

TEST(StateTest, ApplyCurrentVelocity){
    kinematics::State state(0, 0, 2, 3);
    
    state.applyVelocity();
    EXPECT_EQ(state.getPosition(), kinematics::Vector2D(2, 3));
    EXPECT_TRUE(state.getPosition() == kinematics::Vector2D(2, 3));
}

TEST(BodyTest, CreateWithPositionAndVelocity){
    kinematics::Body body(1, 2, 3, 4);
    
    EXPECT_EQ(body.getPosition(), kinematics::Vector2D(1, 2));
    EXPECT_EQ(body.getVelocity(), kinematics::Vector2D(3, 4));
    EXPECT_TRUE(body.getPosition() == kinematics::Vector2D(1, 2));
    EXPECT_TRUE(body.getVelocity() == kinematics::Vector2D(3, 4));
    EXPECT_TRUE(body.getState() == kinematics::State(1, 2, 3, 4));
}

TEST(BodyTest, CreateWithPositionOnly){
    kinematics::Body body(5, 6);
    
    EXPECT_EQ(body.getPosition(), kinematics::Vector2D(5, 6));
    EXPECT_EQ(body.getVelocity(), kinematics::Vector2D(0, 0));
    EXPECT_TRUE(body.getPosition() == kinematics::Vector2D(5, 6));
    EXPECT_TRUE(body.getVelocity() == kinematics::Vector2D(0, 0));
    EXPECT_TRUE(body.getState() == kinematics::State(5, 6, 0, 0));
}

TEST(BodyTest, SetVelocity){
    kinematics::Body body(0, 0, 1, 1);
    
    body.setVelocity(2, 3);
    EXPECT_EQ(body.getVelocity(), kinematics::Vector2D(2, 3));
    EXPECT_TRUE(body.getVelocity() == kinematics::Vector2D(2, 3));
}

TEST(BodyTest, ApplyVelocityAndSetPosition){
    kinematics::Body body(0, 0, 1, 1);

    body.applyVelocity();
    EXPECT_EQ(body.getPosition(), kinematics::Vector2D(1, 1));
    EXPECT_TRUE(body.getPosition() == kinematics::Vector2D(1, 1));
    
    body.setVelocity(2, 3);
    body.applyVelocity();
    EXPECT_EQ(body.getPosition(), kinematics::Vector2D(3, 4));
    EXPECT_TRUE(body.getPosition() == kinematics::Vector2D(3, 4));
}

TEST(BodyTest, UpdateCurrentPosition){
    kinematics::Body body(1, 1, 2, 2);
    
    kinematics::Vector2D* pos = &(body.getPosition());
    *pos += kinematics::Vector2D(3, 4);

    EXPECT_EQ(*pos, kinematics::Vector2D(4, 5));
    EXPECT_EQ(body.getPosition(), kinematics::Vector2D(4, 5));
}

TEST(BodyTest, SetPosition){
    kinematics::Body body(0, 0, 0, 0);
    
    body.setPosition(5, 6);
    EXPECT_EQ(body.getPosition(), kinematics::Vector2D(5, 6));
    EXPECT_TRUE(body.getPosition() == kinematics::Vector2D(5, 6));
}