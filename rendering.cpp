#include "rendering.h"
#include "constants.h"
#include "physics.h"
#include <iostream>
#include <random>
#include <cmath>

namespace Rendering {

// RenderEngine implementation
RenderEngine::RenderEngine(
    int width,
    int height,
    const char* title,
    std::vector<Ray>& raysRef
): rays{&raysRef}, frame{0} {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        std::exit(-1);
    }

    // Create window
    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create window\n";
        glfwTerminate();
        std::exit(-1);
    }

    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        std::exit(-1);
    }

    glViewport(0, 0, Visual::WINDOW_WIDTH, Visual::WINDOW_HEIGHT);

    // Enable visual polish features
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    // Generate background stars
    stars = generateStars(Visual::NUM_STARS);
}

RenderEngine::~RenderEngine() {
    if (window) {
        glfwDestroyWindow(window);
    }
    glfwTerminate();
}

void RenderEngine::beginFrame(float viewWidth, float viewHeight) {
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.02f, 0.02f, 0.05f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-viewWidth, viewWidth, -viewHeight, viewHeight, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void RenderEngine::endFrame() {
    glfwSwapBuffers(window);
    glfwPollEvents();
}

bool RenderEngine::shouldClose() const {
    return glfwWindowShouldClose(window);
}

void RenderEngine::updatePhysics() {
    // Integrate all active rays
    for (auto& ray : *rays) {
        ray.integrate(Simulation::INTEGRATION_STEP, Simulation::MAX_DISTANCE, frame);
    }
    ++frame;
}

void RenderEngine::drawFrame() {
    // Background stars
    drawStars(stars);

    // Photon sphere outline (dashed for visual distinction)
    glColor3f(0.0f, 0.8f, 0.8f);
    glLineWidth(2.0f);
    const float photonRadius{static_cast<float>(1.5 * BlackHole::rs)};
    drawDashedCircle(0.0f, 0.0f, photonRadius, Visual::CIRCLE_SEGMENTS);

    // Event horizon (black)
    glColor3f(0.0f, 0.0f, 0.0f);
    const float eventRadius{static_cast<float>(BlackHole::rs)};
    drawCircle(0.0f, 0.0f, eventRadius, Visual::CIRCLE_SEGMENTS);

    // Mark the point source location
    drawPointSource(Visual::POINT_SOURCE_X, Visual::POINT_SOURCE_Y);

    // Color-coded ray trails
    drawRays(*rays, frame);
}

std::vector<glm::vec2> generateStars(int count) {
    std::vector<glm::vec2> stars;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distX(-Visual::VIEW_WIDTH, Visual::VIEW_WIDTH);
    std::uniform_real_distribution<float> distY(-Visual::VIEW_HEIGHT, Visual::VIEW_HEIGHT);

    for (int i{0}; i < count; ++i) {
        stars.push_back(glm::vec2(distX(gen), distY(gen)));
    }
    return stars;
}

void drawStars(const std::vector<glm::vec2>& stars) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> brightness(0.5f, 1.0f);

    glPointSize(2.0f);
    glBegin(GL_POINTS);
    for (const auto& star : stars) {
        float b = brightness(gen);
        glColor3f(b, b, b);  // Grayscale brightness
        glVertex2f(star.x, star.y);
    }
    glEnd();
}

void drawCircle(float x, float y, float radius, int segments) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);

    for (int i{0}; i <= segments; ++i) {
        const float angle{(static_cast<float>(i) * 2.0f * static_cast<float>(M_PI)) / static_cast<float>(segments)};
        const float px{x + radius * std::cos(angle)};
        const float py{y + radius * std::sin(angle)};
        glVertex2f(px, py);
    }

    glEnd();
}

void drawCircleOutline(float x, float y, float radius, int segments) {
    glBegin(GL_LINE_LOOP);

    for (int i{0}; i < segments; ++i) {
        const float angle{(static_cast<float>(i) * 2.0f * static_cast<float>(M_PI)) / static_cast<float>(segments)};
        const float px{x + radius * std::cos(angle)};
        const float py{y + radius * std::sin(angle)};
        glVertex2f(px, py);
    }

    glEnd();
}

void drawDashedCircle(float x, float y, float radius, int segments) {
    for (int i{0}; i < segments; i += 2) {
        const float theta1{(static_cast<float>(i) * 2.0f * static_cast<float>(M_PI)) / static_cast<float>(segments)};
        const float theta2{(static_cast<float>(i + 1) * 2.0f * static_cast<float>(M_PI)) / static_cast<float>(segments)};

        glBegin(GL_LINES);
        glVertex2f(x + radius * std::cos(theta1), y + radius * std::sin(theta1));
        glVertex2f(x + radius * std::cos(theta2), y + radius * std::sin(theta2));
        glEnd();
    }
}

void drawRays(const std::vector<Ray>& rays, int currentFrame) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glLineWidth(1.5f);

    for (const auto& ray : rays) {
        // Only draw rays that have been activated
        if (!ray.isActive(currentFrame) || ray.trail.size() < 2) {
            continue;
        }

        float r, g, b;

        // Different color schemes for different scenarios
        if (ray.scenario == RayScenario::POINT_SOURCE) {
            // Point source rays: Green to Yellow gradient
            const float maxDeflection{static_cast<float>(M_PI)};
            const float t{std::min(1.0f, static_cast<float>(ray.deflection) / maxDeflection)};
            r = 0.5f + 0.5f * t;  // 0.5 to 1.0
            g = 1.0f;             // Always green
            b = 0.0f;
        } else if (ray.scenario == RayScenario::ORBITING) {
            // Orbiting ray: Bright magenta/purple
            r = 1.0f;
            g = 0.2f;
            b = 1.0f;
        } else {
            // Parallel rays: Blue to red gradient (original)
            const float maxDeflection{static_cast<float>(M_PI)};
            const float t{std::min(1.0f, static_cast<float>(ray.deflection) / maxDeflection)};
            r = t;
            g = 0.5f * (1.0f - t);
            b = 1.0f - t;
        }

        // Draw trail with fading
        glBegin(GL_LINE_STRIP);
        for (size_t i{0}; i < ray.trail.size(); ++i) {
            const float alpha{0.2f + 0.8f * static_cast<float>(i) / static_cast<float>(ray.trail.size() - 1)};

            glColor4f(r, g, b, alpha);
            glVertex2f(ray.trail[i].x, ray.trail[i].y);
        }
        glEnd();
    }

    // Draw current ray positions as bright dots
    glPointSize(3.0f);
    glBegin(GL_POINTS);
    for (const auto& ray : rays) {
        if (ray.isActive(currentFrame) && !ray.trail.empty() && !ray.isCaptured()) {
            // Color-code dots by scenario
            if (ray.scenario == RayScenario::POINT_SOURCE) {
                glColor3f(0.5f, 1.0f, 0.0f);  // Lime green
            } else if (ray.scenario == RayScenario::ORBITING) {
                glColor3f(1.0f, 0.2f, 1.0f);  // Magenta
            } else {
                glColor3f(1.0f, 1.0f, 0.0f);  // Yellow
            }
            glVertex2f(ray.trail.back().x, ray.trail.back().y);
        }
    }
    glEnd();

    glDisable(GL_BLEND);
}

void drawPointSource(float x, float y) {
    glPointSize(8.0f);
    glColor3f(0.5f, 1.0f, 0.0f);
    glBegin(GL_POINTS);
    glVertex2f(x, y);
    glEnd();
}

} // namespace Rendering