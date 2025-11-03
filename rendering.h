#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "ray.h"

// Rendering namespace for all OpenGL drawing operations
namespace Rendering {
    // Generate random background stars
    std::vector<glm::vec2> generateStars(int count);

    // Draw background stars
    void drawStars(const std::vector<glm::vec2>& stars);

    // Draw filled circle (for event horizon)
    void drawCircle(float x, float y, float radius, int segments);

    // Draw circle outline (for photon sphere)
    void drawCircleOutline(float x, float y, float radius, int segments);

    // Draw all active rays with color coding
    void drawRays(const std::vector<Ray>& rays, int currentFrame);

    // Draw point source marker
    void drawPointSource(float x, float y);
}
