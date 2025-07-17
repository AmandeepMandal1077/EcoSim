#ifndef KINEMATICS_H
#define KINEMATICS_H

#include <iostream>

namespace kinematics{
    
    struct Vector2D {
        public:
            int x, y;

            Vector2D(int x, int y)
                :x(x), y(y)
            {}
        
            bool operator==(const Vector2D& other) const { return x == other.x && y == other.y; }
            void operator+=(const Vector2D& other) { x += other.x; y += other.y; }
            void operator-=(const Vector2D& other) { x -= other.x; y -= other.y; }
            void operator*=(const int& scalar) { x *= scalar; y *= scalar; }
            bool operator!=(const Vector2D& other) const { return !(*this == other); }
            
    };
        
    inline std::ostream& operator<<(std::ostream& os, const Vector2D& vec) {
        os << "Vector2D(" << vec.x << ", " << vec.y << ")";
        return os;
    }

    inline Vector2D operator+(const Vector2D& vec1, const Vector2D& vec2) {
        return Vector2D(vec1.x + vec2.x, vec1.y + vec2.y);
    }

    inline Vector2D operator-(const Vector2D& vec1, const Vector2D& vec2) {
        return Vector2D(vec1.x - vec2.x, vec1.y - vec2.y);
    }

    inline Vector2D operator*(const Vector2D& vec, const int& scalar) {
        return Vector2D(vec.x * scalar, vec.y * scalar);
    }

    struct State{
        public:
            State(int posX, int posY, int velX, int velY)
                :position(posX, posY), velocity(velX, velY)
            {}

            Vector2D getPosition() const { return position; }
            Vector2D& getPosition() { return position; }
            Vector2D getVelocity() const { return velocity; }
            Vector2D& getVelocity() { return velocity; }
            
            inline void setVelocity(const int& velX, const int& velY){ velocity = Vector2D(velX, velY); }
            inline void setPosition(const int& posX, const int& posY){ position = Vector2D(posX, posY); }

            inline void applyVelocity(const int& posX, const int& posY){ position += Vector2D(posX, posY); }
            inline void applyVelocity(const Vector2D& vec){ position += vec; }
            inline void applyVelocity(){ position += velocity; }

            bool operator==(const State& other) const {
                return position == other.position && velocity == other.velocity;
            }

        private:
            Vector2D position, velocity;
    };

    class Body{
        public:
            Body(const int& posX, const int& posY, const int& velX, const int& velY)
                :state(posX, posY, velX, velY)
            {}

            Body(const int& posX, const int& posY)
                :state(posX, posY, 0, 0)
            {}

            inline void applyVelocity() { state.applyVelocity(); }
            inline void setVelocity(const int& velX, const int& velY) { state.setVelocity(velX, velY); }
            inline void setPosition(const int& posX, const int& posY) { state.setPosition(posX, posY); }

            inline Vector2D getVelocity() const { return state.getVelocity(); }
            inline Vector2D& getVelocity() { return state.getVelocity(); }
            inline Vector2D getPosition() const { return state.getPosition(); }
            inline Vector2D& getPosition() { return state.getPosition(); }
            inline State getState() const { return state; }

        private:
            State state;
    };
};

#endif