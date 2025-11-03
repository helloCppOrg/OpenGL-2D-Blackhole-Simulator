#pragma once

#include <glm/glm.hpp>
#include <vector>

// Ray scenario types for different visual effects
enum class RayScenario {
    PARALLEL,      // Parallel rays from left side
    POINT_SOURCE,  // Rays emanating from a single point
    ORBITING       // Special orbiting ray
};

// Ray representation in Schwarzschild coordinates
struct Ray {
    // Schwarzschild coordinates
    double r;      // Radial coordinate
    double phi;    // Angular coordinate
    double dr;     // dr/dλ (radial velocity)
    double dphi;   // dφ/dλ (angular velocity)

    // Conserved quantities
    double E;      // Energy per unit mass
    double L;      // Angular momentum per unit mass

    // Visualization data
    std::vector<glm::vec2> trail;
    double initialPhi;
    double deflection;

    // Scenario tracking
    RayScenario scenario;
    int startFrame;

    // Constructor: Initialize ray from Cartesian position and velocity
    Ray(double x, double y, double vx, double vy,
        RayScenario scen = RayScenario::PARALLEL,
        int start = 0);

    // Check if ray has been captured by black hole
    bool isCaptured() const;

    // Check if ray has escaped to infinity
    bool hasEscaped(double maxDistance) const;

    // Update deflection angle (for color coding)
    void updateDeflection();

    // Check if ray should be active at current frame
    bool isActive(int currentFrame) const;
};
