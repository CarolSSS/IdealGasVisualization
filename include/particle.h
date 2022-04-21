#pragma once

#include <glm/vec2.hpp>

namespace idealgas {
    class Particle {
    public:
        /**
          * Constructor to create new particles.
          * @param position starting position of the particle
          * @param velocity starting velocity of the particle
          * @param radius_size radius of the particle
          */
        Particle(glm::vec2 position, glm::vec2 velocity, int radius_size, char* color, float mass);

        /**
         * This method is used to check if current particle struck a wall.
         * @param x_start Start coordinate of x
         * @param x_end End coordinate of x
         * @param y_start Start coordinate of y
         * @param y_end End coordinate of y
         * @return a boolean flag shows whether the particle struck a wall
         */
        bool CheckWall(int x_start, int x_end, int y_start, int y_end);

        /**
         * This method is used to add velocity to position to get the position for next step.
         */
        void UpdatePosition();

        /**
         * This method is used to set velocity from outside.
         * @param velocity Set the velocity of this particle to the input velocity
         */
        void SetVelocity(glm::vec2 velocity);
        const glm::vec2 GetPosition() const;
        const glm::vec2 GetVelocity() const;
        const int GetRadiusSize() const;
        float GetMass() const;
        char* GetColor() const;

    private:
        glm::vec2 position_;
        glm::vec2 velocity_;
        int radius_size_;
        float mass_;
        char* color_;
    };
}