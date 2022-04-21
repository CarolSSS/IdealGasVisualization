#include "gas_container.h"
#include "particle.h"

namespace idealgas {

    using glm::vec2;

    Particle::Particle(vec2 position, vec2 velocity, int radius_size, char* color, float mass) {
        position_ = position;
        velocity_ = velocity;
        radius_size_ = radius_size;
        color_ = color;
        mass_ = mass;
    }

    bool Particle::CheckWall(int x_start, int x_end, int y_start, int y_end) {
        bool return_flag = false;
        if ((position_.x <= (x_start + radius_size_) && (velocity_.x <= 0.0))
            || (position_.x >= (x_end - radius_size_) && (velocity_.x >= 0.0))) {
            velocity_.x = - velocity_.x;
            return_flag = true;
        }
        if ((position_.y <= (y_start + radius_size_) && (velocity_.y <= 0.0))
            || (position_.y >= (y_end - radius_size_)&& (velocity_.y >= 0.0))) {
            velocity_.y = - velocity_.y;
            return_flag = true;
        }
        return return_flag;
    }

    void Particle::SetVelocity(vec2 velocity) {
        velocity_ = velocity;
    }

    void Particle::UpdatePosition() {
        position_ += velocity_;
    }

    const vec2 Particle::GetPosition() const {
        return position_;
    }

    const vec2 Particle::GetVelocity() const {
        return velocity_;
    }

    const int Particle::GetRadiusSize() const {
        return radius_size_;
    }

    float Particle::GetMass() const {
        return mass_;
    }

    char* Particle::GetColor() const {
        return color_;
    }
}