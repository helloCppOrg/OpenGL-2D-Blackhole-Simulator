#include "physics.h"
#include "constants.h"
#include <cmath>

namespace Physics {

// Compute geodesic right-hand side using exact Schwarzschild equations
void geodesicRHS(const Ray& ray, double rhs[4]) {
    double r{ray.r};
    double v_r{ray.v_r};
    double v_phi{ray.v_phi};
    double E_val{ray.E};

    double f{1.0 - BlackHole::rs / r};

    rhs[0] = v_r;
    rhs[1] = v_phi;

    double dt_dlambda{E_val / f};
    rhs[2] = -(BlackHole::rs / (2.0 * r * r)) * f * (dt_dlambda * dt_dlambda)
             + (BlackHole::rs / (2.0 * r * r * f)) * (v_r * v_r)
             + (r - BlackHole::rs) * (v_phi * v_phi);

    rhs[3] = -2.0 * v_r * v_phi / r;
}

// Helper function: add two state vectors
void addState(const double a[4], const double b[4], double factor, double out[4]) {
    for (int i{0}; i < 4; ++i) {
        out[i] = a[i] + b[i] * factor;
    }
}

// RK4 integration step
void rk4Step(Ray& ray, double dlambda) {
    double y0[4]{ray.r, ray.phi, ray.v_r, ray.v_phi};
    double k1[4], k2[4], k3[4], k4[4], temp[4];

    // k1 = f(y0)
    geodesicRHS(ray, k1);

    // k2 = f(y0 + k1*dlambda/2)
    addState(y0, k1, dlambda / 2.0, temp);
    Ray r2{ray};
    r2.r = temp[0];
    r2.phi = temp[1];
    r2.v_r = temp[2];
    r2.v_phi = temp[3];
    geodesicRHS(r2, k2);

    // k3 = f(y0 + k2*dlambda/2)
    addState(y0, k2, dlambda / 2.0, temp);
    Ray r3{ray};
    r3.r = temp[0];
    r3.phi = temp[1];
    r3.v_r = temp[2];
    r3.v_phi = temp[3];
    geodesicRHS(r3, k3);

    // k4 = f(y0 + k3*dlambda)
    addState(y0, k3, dlambda, temp);
    Ray r4{ray};
    r4.r = temp[0];
    r4.phi = temp[1];
    r4.v_r = temp[2];
    r4.v_phi = temp[3];
    geodesicRHS(r4, k4);

    // Update: y_{n+1} = y_n + (k1 + 2k2 + 2k3 + k4) * dlambda/6
    ray.r += (dlambda / 6.0) * (k1[0] + 2.0 * k2[0] + 2.0 * k3[0] + k4[0]);
    ray.phi += (dlambda / 6.0) * (k1[1] + 2.0 * k2[1] + 2.0 * k3[1] + k4[1]);
    ray.v_r += (dlambda / 6.0) * (k1[2] + 2.0 * k2[2] + 2.0 * k3[2] + k4[2]);
    ray.v_phi += (dlambda / 6.0) * (k1[3] + 2.0 * k2[3] + 2.0 * k3[3] + k4[3]);
}

} // namespace Physics