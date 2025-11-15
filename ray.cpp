#include "ray.h"
#include "constants.h"
#include "physics.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Ray::Ray(double x, double y, double vx, double vy, RayScenario scenario, int startFrame)
    : scenario{scenario}, startFrame{startFrame} {
    // Convert to polar coordinates
    r = std::sqrt(x * x + y * y);
    phi = std::atan2(y, x);

    // Store initial velocity direction angle (not position angle)
    initialVelocityAngle = std::atan2(vy, vx);

    // Convert velocity to polar
    v_r = vx * std::cos(phi) + vy * std::sin(phi);
    v_phi = (-vx * std::sin(phi) + vy * std::cos(phi)) / r;

    // Calculate conserved quantities
    L = r * r * v_phi;
    double f{1.0 - BlackHole::rs / r};
    double dt_dlambda{std::sqrt((v_r * v_r) / (f * f) + (r * r * v_phi * v_phi) / f)};
    E = f * dt_dlambda;

    trail.push_back(glm::vec2(static_cast<float>(x), static_cast<float>(y)));
}

bool Ray::isCaptured() const {
    return r <= BlackHole::rs * 1.01;
}

bool Ray::hasEscaped(double maxDistance) const {
    return r > maxDistance;
}

void Ray::recordPosition() {
    const float x{static_cast<float>(r * std::cos(phi))};
    const float y{static_cast<float>(r * std::sin(phi))};
    trail.push_back(glm::vec2(x, y));
}

void Ray::integrate(double dlambda, double maxDistance, int currentFrame) {
    if (!isActive(currentFrame) || isCaptured() || hasEscaped(maxDistance)) {
        return;
    }
    Physics::rk4Step(*this, dlambda);
    recordPosition();
    updateDeflection();
}

void Ray::updateDeflection() {
    // Convert current polar velocity back to Cartesian to get velocity direction
    const double vx_current{v_r * std::cos(phi) - r * v_phi * std::sin(phi)};
    const double vy_current{v_r * std::sin(phi) + r * v_phi * std::cos(phi)};

    // Calculate current velocity direction angle
    const double currentVelocityAngle{std::atan2(vy_current, vx_current)};

    // Deflection = change in velocity direction
    deflection = std::abs(currentVelocityAngle - initialVelocityAngle);

    // Handle angle wrapping (deflections > π map back down)
    if (deflection > M_PI) {
        deflection = 2.0 * M_PI - deflection;
    }
}

bool Ray::isActive(int currentFrame) const {
    return currentFrame >= startFrame;
}