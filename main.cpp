#include <iostream>

// Project headers
#include "constants.h"
#include "ray.h"
#include "rendering.h"

void generateOrbitingRay(std::vector<Ray>& rays) {
    const double orbitStartX{-0.9 * Visual::VIEW_WIDTH};
    const double orbitStartY{2.577934 * BlackHole::rs};
    rays.emplace_back(orbitStartX, orbitStartY, Physics::c, 0.0,
                      RayScenario::ORBITING, Visual::ORBITING_START);
}

void generatePointSourceRays(std::vector<Ray>& rays) {
    const int numPointRays{25};
    const double toBlackHoleX{0.0 - Visual::POINT_SOURCE_X};
    const double toBlackHoleY{0.0 - Visual::POINT_SOURCE_Y};
    const double baseAngle{std::atan2(toBlackHoleY, toBlackHoleX)};

    for (int i{0}; i < numPointRays; ++i) {
        const double coneSpread{M_PI / 3.0};
        const double angleOffset{-coneSpread / 2.0 + coneSpread * static_cast<double>(i) / static_cast<double>(numPointRays - 1)};
        const double angle{baseAngle + angleOffset};
        const double vx{Physics::c * std::cos(angle)};
        const double vy{Physics::c * std::sin(angle)};
        rays.emplace_back(Visual::POINT_SOURCE_X, Visual::POINT_SOURCE_Y, vx, vy,
                          RayScenario::POINT_SOURCE, Visual::POINT_SOURCE_START);
    }
}

void generateParallelRays(std::vector<Ray>& rays) {
    const int numParallelRays{70};
    const double parallelStartX{-1e11};
    for (int i{0}; i < numParallelRays; ++i) {
        const double t{static_cast<double>(i) / static_cast<double>(numParallelRays - 1)};
        const double startY{-Visual::VIEW_HEIGHT + t * 2.0 * Visual::VIEW_HEIGHT};
        rays.emplace_back(parallelStartX, startY, Physics::c, 0.0,
                          RayScenario::PARALLEL, Visual::PARALLEL_START);
    }
}

std::vector<Ray> generateRays() {
    std::vector<Ray> rays;

    generateOrbitingRay(rays);
    generatePointSourceRays(rays);
    generateParallelRays(rays);

    std::cout << "Total rays: " << rays.size() << "\n";

    return rays;
}

int main() {
    std::cout << "\n=== Black Hole Simulation ===\n";

    std::vector<Ray> rays{generateRays()};

    Rendering::RenderEngine engine{
        Visual::WINDOW_WIDTH,
        Visual::WINDOW_HEIGHT,
        "2D Black Hole Simulator - Organized Project",
        rays
    };

    while (!engine.shouldClose()) {
        engine.beginFrame(Visual::VIEW_WIDTH, Visual::VIEW_HEIGHT);
        engine.updatePhysics();
        engine.drawFrame();
        engine.endFrame();
    }

    return 0;
}