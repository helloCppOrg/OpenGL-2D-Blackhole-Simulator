#include "physics.h"
#include "constants.h"
#include <cmath>

namespace Physics {

void geodesicRHS(const Ray& ray, double rhs[4]) {
    double r{ray.r};
    double dr_val{ray.dr};
    double dphi_val{ray.dphi};
    double E_val{ray.E};

    double f{1.0 - BlackHole::rs / r};

    // Position derivatives
    rhs[0] = dr_val;
    rhs[1] = dphi_val;

    // Velocity derivatives (geodesic equations)
    double dt_dlambda{E_val / f};
    rhs[2] = -(BlackHole::rs / (2.0 * r * r)) * f * (dt_dlambda * dt_dlambda)
             + (BlackHole::rs / (2.0 * r * r * f)) * (dr_val * dr_val)
             + (r - BlackHole::rs) * (dphi_val * dphi_val);

    rhs[3] = -2.0 * dr_val * dphi_val / r;
}

void addState(const double a[4], const double b[4], double factor, double out[4]) {
    for (int i{0}; i < 4; ++i) {
        out[i] = a[i] + b[i] * factor;
    }
}

void rk4Step(Ray& ray, double dlambda) {
    if (ray.isCaptured()) {
        return;
    }

    double y0[4]{ray.r, ray.phi, ray.dr, ray.dphi};
    double k1[4], k2[4], k3[4], k4[4], temp[4];

    // RK4 stages
    geodesicRHS(ray, k1);

    addState(y0, k1, dlambda / 2.0, temp);
    Ray r2{ray};
    r2.r = temp[0];
    r2.phi = temp[1];
    r2.dr = temp[2];
    r2.dphi = temp[3];
    geodesicRHS(r2, k2);

    addState(y0, k2, dlambda / 2.0, temp);
    Ray r3{ray};
    r3.r = temp[0];
    r3.phi = temp[1];
    r3.dr = temp[2];
    r3.dphi = temp[3];
    geodesicRHS(r3, k3);

    addState(y0, k3, dlambda, temp);
    Ray r4{ray};
    r4.r = temp[0];
    r4.phi = temp[1];
    r4.dr = temp[2];
    r4.dphi = temp[3];
    geodesicRHS(r4, k4);

    // Update state with weighted average
    ray.r += (dlambda / 6.0) * (k1[0] + 2.0 * k2[0] + 2.0 * k3[0] + k4[0]);
    ray.phi += (dlambda / 6.0) * (k1[1] + 2.0 * k2[1] + 2.0 * k3[1] + k4[1]);
    ray.dr += (dlambda / 6.0) * (k1[2] + 2.0 * k2[2] + 2.0 * k3[2] + k4[2]);
    ray.dphi += (dlambda / 6.0) * (k1[3] + 2.0 * k2[3] + 2.0 * k3[3] + k4[3]);

    // Add new position to trail
    const float x{static_cast<float>(ray.r * std::cos(ray.phi))};
    const float y{static_cast<float>(ray.r * std::sin(ray.phi))};
    ray.trail.push_back(glm::vec2(x, y));

    // Update deflection for color coding
    ray.updateDeflection();
}

} // namespace Physics
