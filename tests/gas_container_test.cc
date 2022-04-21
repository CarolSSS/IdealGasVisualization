#include <catch2/catch.hpp>

#include <gas_container.h>

using idealgas::GasContainer;

TEST_CASE("Create new particle") {
    SECTION("Create new particle in empty vector") {
        GasContainer current_container = GasContainer();
        std::vector<idealgas::Particle> new_particles;
        current_container.SetParticles(new_particles);
        current_container.CreateNewParticle(vec2(200, 200), vec2(2, 2), 10,
                                            (char *)"blue", 0.1);
        REQUIRE(current_container.GetParticles().at(0).GetPosition() == vec2(200, 200));
    }
}

TEST_CASE("Test particle collides wall") {
    SECTION("Normally without collision") {
        GasContainer current_container = GasContainer();
        std::vector<idealgas::Particle> new_particles;
        current_container.SetParticles(new_particles);
        current_container.CreateNewParticle(vec2(200, 200), vec2(2, 2), 10,
                                            (char *) "green", 0.1);
        current_container.AdvanceOneFrame();
        REQUIRE(current_container.GetParticles().at(0).GetPosition() == vec2(202, 202));
    }
    SECTION("Collide wall left and up") {
        GasContainer current_container = GasContainer();
        std::vector<idealgas::Particle> new_particles;
        current_container.SetParticles(new_particles);
        current_container.CreateNewParticle(vec2(100, 100), vec2(-1, -1), 10,
                                            (char *) "blue", 0.1);
        current_container.AdvanceOneFrame();
        REQUIRE(current_container.GetParticles().at(0).GetPosition() == vec2(101, 101));
    }
    SECTION("Collide wall right") {
        GasContainer current_container = GasContainer();
        std::vector<idealgas::Particle> new_particles;
        current_container.SetParticles(new_particles);
        current_container.CreateNewParticle(vec2(600, 100), vec2(1, 1), 10,
                                            (char *) "red", 5);
        current_container.AdvanceOneFrame();
        REQUIRE(current_container.GetParticles().at(0).GetPosition() == vec2(599, 101));
    }
    SECTION("Collide wall down") {
        GasContainer current_container = GasContainer();
        std::vector<idealgas::Particle> new_particles;
        current_container.SetParticles(new_particles);
        current_container.CreateNewParticle(vec2(100, 400), vec2(1, 1), 10,
                                            (char *) "blue", 1);
        current_container.AdvanceOneFrame();
        REQUIRE(current_container.GetParticles().at(0).GetPosition() == vec2(101, 399));
    }
}
TEST_CASE("Test Collision between particles") {
    SECTION("Two points collision different direction") {
        GasContainer current_container = GasContainer();
        std::vector<idealgas::Particle> new_particles;
        current_container.SetParticles(new_particles);
        current_container.CreateNewParticle(vec2(200, 200), vec2(-3, -3), 10,
                                            (char *)"blue", 0.9);
        current_container.CreateNewParticle(vec2(189, 189), vec2(2, 2), 10,
                                            (char *)"blue", 0.1);
        current_container.AdvanceOneFrame();
        REQUIRE(current_container.GetParticles().at(0).GetPosition() == vec2(198, 198));
        REQUIRE(current_container.GetParticles().at(1).GetPosition() == vec2(182, 182));
    }
    SECTION("Two points collision but already move away") {
        GasContainer current_container = GasContainer();
        std::vector<idealgas::Particle> new_particles;
        current_container.SetParticles(new_particles);
        current_container.CreateNewParticle(vec2(200, 200), vec2(3, 3), 10,
                                            (char *)"blue", 1);
        current_container.CreateNewParticle(vec2(195, 195), vec2(-2, -2), 5,
                                            (char *)"yellow", 0.1);
        current_container.AdvanceOneFrame();
        REQUIRE(current_container.GetParticles().at(0).GetPosition() == vec2(203, 203));
        REQUIRE(current_container.GetParticles().at(1).GetPosition() == vec2(193, 193));
    }
    SECTION("Three points collision") {
        GasContainer current_container = GasContainer();
        std::vector<idealgas::Particle> new_particles;
        current_container.SetParticles(new_particles);
        current_container.CreateNewParticle(vec2(200, 200), vec2(-3, -3), 10,
                                            (char *)"blue", 0.9);
        current_container.CreateNewParticle(vec2(189, 189), vec2(2, 2), 10,
                                            (char *)"blue", 0.1);
        current_container.CreateNewParticle(vec2(210, 210), vec2(-4, -4), 5,
                                            (char *)"yellow", 0.1);
        current_container.AdvanceOneFrame();
        REQUIRE(current_container.GetParticles().at(0).GetPosition() == vec2(197.6, 197.6));
        REQUIRE(current_container.GetParticles().at(1).GetPosition() == vec2(182, 182));
        REQUIRE(current_container.GetParticles().at(2).GetPosition() == vec2(209.6, 209.6));
    }
}

TEST_CASE("Test Histogram correctness") {
    SECTION("Test if data point is in correct place") {
        GasContainer current_container = GasContainer();
        std::vector<idealgas::Particle> new_particles;
        current_container.SetParticles(new_particles);
        // Simulate a graph
        current_container.CreateNewParticle(vec2(100, 200), vec2(2, 2),
                                            10, (char *)"green", 0.1);
        std::map<float, int> current_map = current_container.CalculateBin(0.1);
        REQUIRE(current_map[4.0] == 0);
        REQUIRE(current_map[3.0] == 0);
        REQUIRE(current_map[2.0] == 1);
        REQUIRE(current_map[1.0] == 0);
    }
    SECTION("Test if bin number is correct and only calculate with the specified mass") {
        GasContainer current_container = GasContainer();
        std::vector<idealgas::Particle> new_particles;
        current_container.SetParticles(new_particles);
        // Simulate a graph
        current_container.CreateNewParticle(vec2(100, 200), vec2(2, 2),
                                            10, (char *)"green", 0.3);
        current_container.CreateNewParticle(vec2(200, 200), vec2(1, 1),
                                            10, (char *)"green", 1);
        current_container.CreateNewParticle(vec2(200, 300), vec2(1, 1),
                                            10, (char *)"green", 1);
        current_container.CreateNewParticle(vec2(200, 400), vec2(1, 1),
                                            10, (char *)"green", 1);
        current_container.CreateNewParticle(vec2(300, 200), vec2(1, 1),
                                            10, (char *)"green", 0.3);
        current_container.CreateNewParticle(vec2(320, 220), vec2(1, 1),
                                            10, (char *)"green", 0.3);
        current_container.CreateNewParticle(vec2(400, 200), vec2(5, 5),
                                            10, (char *)"green", 0.3);
        current_container.CreateNewParticle(vec2(400, 300), vec2(2, 2),
                                            10, (char *)"green", 3);
        std::map<float, int> current_map = current_container.CalculateBin(0.3);
        REQUIRE(current_map[7.0] == 1);
        REQUIRE(current_map[6.0] == 0);
        REQUIRE(current_map[5.0] == 0);
        REQUIRE(current_map[4.0] == 0);
        REQUIRE(current_map[3.0] == 0);
        REQUIRE(current_map[2.0] == 1);
        REQUIRE(current_map[1.0] == 2);
    }
}

