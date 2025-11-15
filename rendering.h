#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include "ray.h"

// Rendering namespace for all OpenGL drawing operations
namespace Rendering {
    // RenderEngine handles all GLFW/OpenGL initialization and frame rendering
    struct RenderEngine {
        GLFWwindow* window;
        std::vector<glm::vec2> stars;
        std::vector<Ray>* rays;
        int frame;

        // Initialize GLFW, GLEW, and create window with rendering state
        RenderEngine(int width, int height, const char* title,
                     std::vector<Ray>& raysRef);

        // Cleanup
        ~RenderEngine();

        // Setup projection and clear screen (call at start of each frame)
        void beginFrame(float viewWidth, float viewHeight);

        // Update physics simulation for all rays
        void updatePhysics();

        // Draw entire scene (stars, black hole, rays, point source)
        void drawFrame();

        // Swap buffers and poll events (call at end of each frame)
        void endFrame();

        // Check if window should close
        bool shouldClose() const;
    };

    // Generate random background stars
    std::vector<glm::vec2> generateStars(int count);

    // Draw background stars
    void drawStars(const std::vector<glm::vec2>& stars);

    // Draw filled circle (for event horizon)
    void drawCircle(float x, float y, float radius, int segments);

    // Draw circle outline (for photon sphere)
    void drawCircleOutline(float x, float y, float radius, int segments);

    // Draw dashed circle (for photon sphere with visual distinction)
    void drawDashedCircle(float x, float y, float radius, int segments);

    // Draw all active rays with color coding
    void drawRays(const std::vector<Ray>& rays, int currentFrame);

    // Draw point source marker
    void drawPointSource(float x, float y);
}