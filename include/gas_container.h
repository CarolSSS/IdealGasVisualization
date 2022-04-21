#pragma once

#include "cinder/gl/gl.h"
#include "particle.h"

using glm::vec2;

namespace idealgas {

/**
 * The container in which all of the gas particles are contained. This class
 * stores all of the particles and updates them on each frame of the simulation.
 */
class GasContainer {
 public:
  /**
   * Initialize gas container with random number.
   */
  GasContainer();

  /**
   * Displays the container walls and the current positions of the particles.
   */
  void Display();

  /**
   * Updates the positions and velocities of all particles (based on the rules
   * described in the assignment documentation).
   */
  void AdvanceOneFrame();

  /**
   * Getter for private variable particles_
   */
  const std::vector<idealgas::Particle> &GetParticles() const;

  /**
   * Setter for private variable particles_
  */
  void SetParticles(const std::vector<idealgas::Particle> &particles);

  /**
   * The method is called in constructor to create new particles.
   * @param position starting position of the particle
   * @param velocity starting velocity of the particle
   * @param radius_size radius of the particle
  */
  void CreateNewParticle(vec2 position, vec2 velocity, int radius_size, char* color, float mass);

  /**
   * This method is used to return a counter of bins for histogram
   * @param current_mass The mass of the particle
   * @return The counter of each bins
   */
  std::map<float, int> CalculateBin(float current_mass);

private:
  // Vector of all particles in the container
  std::vector<idealgas::Particle> particles_;
  // Default board
  const int SQUARE_X_START = 100;
  const int SQUARE_X_END = 600;
  const int SQUARE_Y_START = 100;
  const int SQUARE_Y_END = 400;
  // Particle infos
  const int MAX_PARTICLE = 101;
  const int MIN_PARTICLE = 20;
  const int INCREMENT_SIZE_X = 5;
  const int INCREMENT_SIZE_Y = 2;
  const int DEFAULT_RADIUS = 10;
  const int MAX_SPEED = 3;
  // Histogram infos
  const int BOX_HEIGHT = 100;
  const int HISTOGRAM_X_START = 720;
  const int HISTOGRAM_X_END = 1020;
  const int SPACE_THRESHOLD = 10;
  const int HISTOGRAM_SPACE_THRESHOLD = 50;
  const float SPEED_THRESHOLD = 1;
  const int SPEED_Y_AXES = 4;
  const int SPEED_X_AXES = 20;
  const int GROUP_SIZE = 15;
  // An array stores mass
  float mass_array_[3] = {0.3, 1, 3};

  /**
   * The method is used to check the distance between two particles.
   * @param particle_first First particle to detect
   * @param particle_second Second particle to detect
   * @return The distance in float
  */
  float GetDistance(glm::vec2& particle_first, glm::vec2& particle_second);

  /**
   * The method is used to check the velocity after collision for first particle.
   * @param particle_first First particle to detect collision
   * @param particle_second Second particle to detect collision
   * @return The velocity of the first particle
  */
  glm::vec2 CalculateVelocity(Particle& particle_first, Particle& particle_second);

  /**
   * This method is used to draw histogram based on its mass.
   * @param number the group which is ready to be drawn.
  */
  void DrawHistogram(int number);
};
}  // namespace idealgas
