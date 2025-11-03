#include "ray.h"
#include "constants.h"
#include <cmath>

Ray::Ray(double x, double y, double vx, double vy, RayScenario scen, int start)
    : scenario{scen}, startFrame{start} {

    // Convert Cartesian to polar coordinates
    r = std::sqrt(x * x + y * y);
    phi = std::atan2(y, x);
    initialPhi = phi;
    deflection = 0.0;

    // Convert Cartesian velocity to polar velocity
    dr = vx * std::cos(phi) + vy * std::sin(phi);
    dphi = (-vx * std::sin(phi) + vy * std::cos(phi)) / r;

    // Calculate conserved quantities
    L = r * r * dphi;
    double f{1.0 - BlackHole::rs / r};
    double dt_dlambda{std::sqrt((dr * dr) / (f * f) + (r * r * dphi * dphi) / f)};
    E = f * dt_dlambda;

    // Initialize trail
    trail.push_back(glm::vec2(static_cast<float>(x), static_cast<float>(y)));
}

bool Ray::isCaptured() const {
    return r <= BlackHole::rs * 1.01;
}

bool Ray::hasEscaped(double maxDistance) const {
    return r > maxDistance;
}

void Ray::updateDeflection() {
    deflection = std::abs(phi - initialPhi);
}

bool Ray::isActive(int currentFrame) const {
    return currentFrame >= startFrame;
}
