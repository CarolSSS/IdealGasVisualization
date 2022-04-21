#include <catch2/catch.hpp>

#include <gas_simulation_app.h>

using idealgas::Particle;

TEST_CASE("Test collide wall") {
    SECTION("Return flag of colliding x start wall") {
        Particle current = Particle(vec2(1,3), vec2(-1, 0), 1, (char *)"blue", 0.1);
        REQUIRE(current.CheckWall(1, 10, 1, 10) == true);
    }
    SECTION("Return flag of not colliding the wall (Come from outside)") {
        Particle current = Particle(vec2(3,3), vec2(1, 1), 1, (char *)"blue", 0.1);
        REQUIRE(current.CheckWall(1, 10, 1, 10) == false);
    }
    SECTION("Return flag of collision but same direction") {
        Particle current = Particle(vec2(1,1), vec2(1, 1), 1, (char *)"blue", 0.1);
        REQUIRE(current.CheckWall(1, 10, 1, 10) == false);
    }
    SECTION("Velocity change of colliding two walls together") {
        Particle current = Particle(vec2(1,1), vec2(-1, -1), 1, (char *)"blue", 0.1);
        current.CheckWall(1, 10, 1, 10);
        REQUIRE(current.GetVelocity() == vec2(1,1));
    }
}

TEST_CASE("Set Velocity") {
    SECTION("Return flag of colliding x start wall") {
        Particle current = Particle(vec2(1,3), vec2(-1, 0), 1, (char *)"blue", 0.1);
        current.SetVelocity(vec2(2, 2));
        REQUIRE(current.GetVelocity() == vec2(2, 2));
    }
}

TEST_CASE("Update Position") {
    SECTION("Return flag of colliding x start wall") {
        Particle current = Particle(vec2(1,3), vec2(-1, 0), 1, (char *)"blue", 0.1);
        current.UpdatePosition();
        REQUIRE(current.GetPosition() == vec2(0, 3));
    }
}