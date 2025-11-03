#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

// Project headers
#include "constants.h"
#include "ray.h"
#include "physics.h"
#include "rendering.h"

// Step 08: Project Organization - Refactored from Step 07
// This demonstrates proper multi-file C++ project structure

void printPhysicsInfo() {
    std::cout << "\n=== Black Hole Simulation ===\n";
    std::cout << "Schwarzschild radius: " << BlackHole::rs / 1000.0 << " km\n";
    std::cout << "Speed of light: " << Physics::c / 1e8 << " * 10^8 m/s\n\n";
}

std::vector<Ray> generateRays() {
    std::vector<Ray> rays;

    // SCENARIO 1: Special orbiting ray near photon sphere (1 ray)
    // Starts at frame 0 from left side of screen
    std::cout << "=== Scenario 1: Orbiting Ray ===\n";

    const double orbitStartX{-0.9 * Visual::VIEW_WIDTH};
    const double orbitStartY{2.577934 * BlackHole::rs};  // Just below critical for multiple spirals

    const double orbitVx{Physics::c};
    const double orbitVy{0.0};

    rays.emplace_back(orbitStartX, orbitStartY, orbitVx, orbitVy,
                      RayScenario::ORBITING, Visual::ORBITING_START);

    std::cout << "  Orbiting ray starts at x = " << orbitStartX / 1e9 << " Gm\n";
    std::cout << "  Impact parameter = " << orbitStartY / BlackHole::rs << " rs\n";
    std::cout << "  Starts at frame: " << Visual::ORBITING_START << "\n";
    std::cout << "  Expected: Multiple dramatic spiral orbits\n";

    // SCENARIO 2: Point source from top-left corner (25 rays)
    // Starts at frame 700
    std::cout << "\n=== Scenario 2: Point Source from Top-Left ===\n";

    const double sourceX{-0.95 * Visual::VIEW_WIDTH};
    const double sourceY{0.85 * Visual::VIEW_HEIGHT};
    const int numPointRays{25};

    // Calculate direction from source to black hole (at origin)
    const double toBlackHoleX{0.0 - sourceX};
    const double toBlackHoleY{0.0 - sourceY};
    const double baseAngle{std::atan2(toBlackHoleY, toBlackHoleX)};

    std::cout << "  Source position: (" << sourceX / 1e9 << ", " << sourceY / 1e9 << ") Gm\n";
    std::cout << "  Aiming toward black hole at angle: " << baseAngle * 180.0 / M_PI << " degrees\n";
    std::cout << "  Starts at frame: " << Visual::POINT_SOURCE_START << "\n";

    for (int i{0}; i < numPointRays; ++i) {
        const double coneSpread{M_PI / 3.0};  // 60 degrees total
        const double angleOffset{-coneSpread / 2.0 + coneSpread * static_cast<double>(i) / static_cast<double>(numPointRays - 1)};
        const double angle{baseAngle + angleOffset};

        const double vx{Physics::c * std::cos(angle)};
        const double vy{Physics::c * std::sin(angle)};

        rays.emplace_back(sourceX, sourceY, vx, vy,
                          RayScenario::POINT_SOURCE, Visual::POINT_SOURCE_START);

        if (i % 5 == 0) {
            std::cout << "  Point ray " << i << ": angle = " << angle * 180.0 / M_PI << " degrees\n";
        }
    }

    // SCENARIO 3: Parallel rays from left (70 rays)
    // Starts at frame 1200
    std::cout << "\n=== Scenario 3: Parallel Rays ===\n";

    const int numParallelRays{70};
    const double parallelStartX{-1e11};  // 100 million km to the left
    const double parallelVx{Physics::c};
    const double parallelVy{0.0};

    std::cout << "  Starts at frame: " << Visual::PARALLEL_START << "\n";

    for (int i{0}; i < numParallelRays; ++i) {
        const double t{static_cast<double>(i) / static_cast<double>(numParallelRays - 1)};
        const double startY{-Visual::VIEW_HEIGHT + t * 2.0 * Visual::VIEW_HEIGHT};

        rays.emplace_back(parallelStartX, startY, parallelVx, parallelVy,
                          RayScenario::PARALLEL, Visual::PARALLEL_START);

        if (i % 10 == 0) {
            const double impactParam{std::abs(startY)};
            std::cout << "  Parallel ray " << i << ": y = " << startY / 1e9
                      << " Gm, impact = " << impactParam / BlackHole::rs << " rs\n";
        }
    }

    std::cout << "\n=== Total: " << rays.size() << " rays ===\n";
    std::cout << "  1 orbiting (magenta) - starts frame " << Visual::ORBITING_START << "\n";
    std::cout << "  " << numPointRays << " point source (green-yellow gradient) - starts frame " << Visual::POINT_SOURCE_START << "\n";
    std::cout << "  " << numParallelRays << " parallel (blue-red gradient) - starts frame " << Visual::PARALLEL_START << "\n\n";

    return rays;
}

void printProgress(const std::vector<Ray>& rays, int frame) {
    int activated{0};
    int active{0};
    int captured{0};

    for (const auto& ray : rays) {
        if (ray.isActive(frame)) {
            ++activated;
            if (ray.isCaptured()) {
                ++captured;
            } else if (!ray.hasEscaped(Simulation::MAX_DISTANCE)) {
                ++active;
            }
        }
    }

    std::cout << "Frame " << frame << ": " << activated << " activated, "
              << active << " active, " << captured << " captured\n";
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    // Create window
    GLFWwindow* window{glfwCreateWindow(
        Visual::WINDOW_WIDTH, Visual::WINDOW_HEIGHT,
        "2D Black Hole Simulator - Organized Project",
        nullptr, nullptr)};

    if (!window) {
        std::cerr << "Failed to create window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    glViewport(0, 0, Visual::WINDOW_WIDTH, Visual::WINDOW_HEIGHT);

    // Enable visual polish features
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    // Print simulation info
    printPhysicsInfo();

    // Generate background stars
    std::vector<glm::vec2> stars{Rendering::generateStars(Visual::NUM_STARS)};
    std::cout << "Generated " << Visual::NUM_STARS << " background stars\n\n";

    // Generate all rays
    std::vector<Ray> rays{generateRays()};

    // Store point source location for rendering
    const float sourceX{-0.95f * Visual::VIEW_WIDTH};
    const float sourceY{0.85f * Visual::VIEW_HEIGHT};

    // Setup projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-Visual::VIEW_WIDTH, Visual::VIEW_WIDTH,
            -Visual::VIEW_HEIGHT, Visual::VIEW_HEIGHT,
            -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    int frame{0};

    std::cout << "=== Simulation Starting ===\n";
    std::cout << "Watch for sequential activation:\n";
    std::cout << "  Frame " << Visual::ORBITING_START << ": Magenta orbiting ray begins\n";
    std::cout << "  Frame " << Visual::POINT_SOURCE_START << ": Green-yellow point source rays appear\n";
    std::cout << "  Frame " << Visual::PARALLEL_START << ": Blue-red parallel rays fill the screen\n\n";

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Dark space background
        glClearColor(0.02f, 0.02f, 0.05f, 1.0f);

        // Layer 1: Background stars
        Rendering::drawStars(stars);

        // Layer 2: Photon sphere outline (bright cyan)
        glColor3f(0.0f, 0.8f, 0.8f);
        glLineWidth(2.0f);
        const float photonRadius{static_cast<float>(1.5 * BlackHole::rs)};
        Rendering::drawCircleOutline(0.0f, 0.0f, photonRadius, Visual::CIRCLE_SEGMENTS);

        // Layer 3: Event horizon (black)
        glColor3f(0.0f, 0.0f, 0.0f);
        const float eventRadius{static_cast<float>(BlackHole::rs)};
        Rendering::drawCircle(0.0f, 0.0f, eventRadius, Visual::CIRCLE_SEGMENTS);

        // Layer 3.5: Mark the point source location
        Rendering::drawPointSource(sourceX, sourceY);

        // Integrate all active rays
        for (auto& ray : rays) {
            if (ray.isActive(frame) && !ray.isCaptured() && !ray.hasEscaped(Simulation::MAX_DISTANCE)) {
                Physics::rk4Step(ray, Simulation::INTEGRATION_STEP);
            }
        }

        // Layer 4: Color-coded ray trails
        Rendering::drawRays(rays, frame);

        // Print progress occasionally
        if (frame % Simulation::PROGRESS_INTERVAL == 0) {
            printProgress(rays, frame);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();

        ++frame;
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
