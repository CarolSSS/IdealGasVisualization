#include "gas_container.h"
#include "particle.h"

namespace idealgas {

using glm::vec2;

GasContainer::GasContainer() {
    // reference: https://www.tutorialspoint.com/c_standard_library/c_function_rand.htm
    // Using random to generate particles
    time_t t;
    srand((unsigned) time(&t));
    for (int i = MIN_PARTICLE; i < MAX_PARTICLE; i++) {
        // The rule for generation: Divide the particle with the mod of 3
        // The starting speed would be random, color would be different, mass would be same within the group
        if (i % MAX_SPEED == 0) {
            CreateNewParticle(vec2(SQUARE_X_START + INCREMENT_SIZE_X * i,
                                   SQUARE_Y_START + INCREMENT_SIZE_Y * i),
                              vec2(rand() % MAX_SPEED, rand() % MAX_SPEED),
                              DEFAULT_RADIUS, (char *)"blue", mass_array_[0]);
        } else if (i % MAX_SPEED == 1) {
            CreateNewParticle(vec2(SQUARE_X_START + INCREMENT_SIZE_X * i,
                                   SQUARE_Y_START + INCREMENT_SIZE_Y * i),
                              vec2(rand() % MAX_SPEED, rand() % MAX_SPEED),
                              DEFAULT_RADIUS, (char *)"red", mass_array_[1]);
        } else {
            CreateNewParticle(vec2(SQUARE_X_START + INCREMENT_SIZE_X * i,
                                   SQUARE_Y_START + INCREMENT_SIZE_Y * i),
                              vec2(rand() % MAX_SPEED, rand() % MAX_SPEED),
                              rand() % MAX_SPEED * DEFAULT_RADIUS, (char *)"pink", mass_array_[2]);
        }
    }
}

void GasContainer::CreateNewParticle(vec2 position, vec2 velocity, int radius_size, char* color, float mass) {
    particles_.push_back(Particle(position, velocity, radius_size, color, mass));
}

void GasContainer::Display() {
    // Draw box
    for (Particle particle : particles_) {
        ci::gl::color(ci::Color((const char *) particle.GetColor()));
        ci::gl::drawSolidCircle(particle.GetPosition(), particle.GetRadiusSize());
    }
    ci::gl::color(ci::Color("white"));
    ci::gl::drawStrokedRect(ci::Rectf(vec2(SQUARE_X_START, SQUARE_Y_START),
                                      vec2(SQUARE_X_END, SQUARE_Y_END)));
    // Draw histograms
    GasContainer::DrawHistogram(0);
    GasContainer::DrawHistogram(1);
    GasContainer::DrawHistogram(2);
}

void GasContainer::DrawHistogram(int number) {
    int start_point = number * (BOX_HEIGHT + HISTOGRAM_SPACE_THRESHOLD);
    float current_mass = mass_array_[number];
    std::string title = "Particle of mass " + std::to_string(current_mass);
    ci::gl::drawStrokedRect(ci::Rectf(vec2(HISTOGRAM_X_START, SQUARE_Y_START + start_point),
                                      vec2(HISTOGRAM_X_END, SQUARE_Y_START + BOX_HEIGHT + start_point)));
    ci::gl::drawStringCentered(title, vec2((HISTOGRAM_X_START + HISTOGRAM_X_END) / 2,
                                             SQUARE_Y_START + start_point - SPACE_THRESHOLD), "yellow");
    ci::gl::drawStringRight("Frequency ", vec2(HISTOGRAM_X_START,
                                                        SQUARE_Y_START + start_point + BOX_HEIGHT / 2));
    ci::gl::drawStringCentered("Speed", vec2((HISTOGRAM_X_START + HISTOGRAM_X_END) / 2,
                                             SQUARE_Y_START + BOX_HEIGHT+ start_point + SPACE_THRESHOLD));
    // A map to record the number of each bin
    std::map<float, int> speed_counter = CalculateBin(current_mass);
    int start_position = SQUARE_Y_START + start_point + BOX_HEIGHT;
    for (size_t i = 0; i < GROUP_SIZE; i++) {
        // The height of each bin would be speed_counter[i] * SPEED_Y_AXES (A constant threshold)
        ci::gl::drawSolidRect(ci::Rectf(vec2(HISTOGRAM_X_START + i * SPEED_X_AXES,start_position),
                                        vec2(HISTOGRAM_X_START  + i * SPEED_X_AXES + SPEED_X_AXES,
                                             start_position - speed_counter[i] * SPEED_Y_AXES)));
    }
}

std::map<float, int> GasContainer::CalculateBin(float current_mass) {
    std::map<float, int> speed_counter;
    for (size_t i = 0; i < particles_.size(); i++) {
        // Check if current particle is the one with ideal mass
        if (particles_[i].GetMass() == current_mass) {
            float new_speed = glm::length(particles_[i].GetVelocity());
            // Merge particles with same integer part to a same bin
            int group = (int)(new_speed / SPEED_THRESHOLD);
            speed_counter[group]++;
        }
    }
    return speed_counter;
}

void GasContainer::AdvanceOneFrame() {
    // Update box
    for (size_t i = 0; i < particles_.size(); i++) {
        vec2 potential_i = particles_[i].GetPosition() + particles_[i].GetVelocity();
        // Change velocity while about to collide
        for (size_t j = i + 1; j < particles_.size(); j++) {
            vec2 potential_j = particles_[j].GetPosition() + particles_[j].GetVelocity();
            // Detect collision, change velocity but not moving
            if (GetDistance(potential_i, potential_j)
                <= (particles_[i].GetRadiusSize() + particles_[j].GetRadiusSize())) {
                vec2 new_velocity_i = CalculateVelocity(particles_[i], particles_[j]);
                vec2 new_velocity_j = CalculateVelocity(particles_[j], particles_[i]);
                particles_[i].SetVelocity(new_velocity_i);
                particles_[j].SetVelocity(new_velocity_j);
            }
        }
        // Check wall and update position
        particles_[i].Particle::CheckWall(SQUARE_X_START, SQUARE_X_END,
                                             SQUARE_Y_START, SQUARE_Y_END);
        particles_[i].UpdatePosition();
    }
}


float GasContainer::GetDistance(glm::vec2& particle_first, glm::vec2& particle_second) {
    return sqrt(std::pow(particle_first.x - particle_second.x, 2)
            + std::pow(particle_first.y - particle_second.y, 2));
}

vec2 GasContainer::CalculateVelocity(Particle& particle_first, Particle& particle_second) {
    vec2 v1 = particle_first.GetVelocity();
    vec2 v2 = particle_second.GetVelocity();
    vec2 x1 = particle_first.GetPosition();
    vec2 x2 = particle_second.GetPosition();
    float m1 = particle_first.GetMass();
    float m2 = particle_second.GetMass();
    float upper_part = glm::dot((v1 - v2),(x1 - x2));
    vec2 diff = x1 - x2;
    // When they are in the same direction
    if (upper_part >= 0) {
        return v1;
    }
    return v1 - (2 * m2 / (m1 + m2)) * (upper_part / glm::length2(diff)) * diff;
}

const std::vector<idealgas::Particle> &GasContainer::GetParticles() const {
    return particles_;
}

void GasContainer::SetParticles(const std::vector<idealgas::Particle> &particles_input) {
    GasContainer::particles_ = particles_input;
}
}  // namespace idealgas
