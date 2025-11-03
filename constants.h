#pragma once

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Physical constants (SI units)
namespace Physics {
    constexpr double c{299792458.0};        // Speed of light (m/s)
    constexpr double G{6.67430e-11};        // Gravitational constant
}

// Black hole parameters
namespace BlackHole {
    constexpr double MASS{8.54e36};         // Sagittarius A* mass (kg)
    const double rs{2.0 * Physics::G * MASS / (Physics::c * Physics::c)};  // Schwarzschild radius
}

// Visual configuration
namespace Visual {
    constexpr int NUM_STARS{200};
    constexpr int CIRCLE_SEGMENTS{100};

    // Window settings
    constexpr int WINDOW_WIDTH{800};
    constexpr int WINDOW_HEIGHT{600};

    // Viewport in physical coordinates
    constexpr float VIEW_WIDTH{100000000000.0f};   // 100 Gm
    constexpr float VIEW_HEIGHT{75000000000.0f};   // 75 Gm

    // Ray timing (frames)
    constexpr int ORBITING_START{0};
    constexpr int POINT_SOURCE_START{700};
    constexpr int PARALLEL_START{1200};
}

// Simulation parameters
namespace Simulation {
    constexpr double MAX_DISTANCE{2e11};    // Maximum escape distance
    constexpr double INTEGRATION_STEP{1.0}; // RK4 time step
    constexpr int PROGRESS_INTERVAL{100};   // Frames between progress prints
}
