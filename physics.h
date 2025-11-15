#pragma once

#include "ray.h"

// Physics simulation namespace
namespace Physics {
    // Calculate geodesic equations for Schwarzschild metric
    // Fills rhs array with [dr/dλ, dφ/dλ, d²r/dλ², d²φ/dλ²]
    void geodesicRHS(const Ray& ray, double rhs[4]);

    // Helper function for RK4 integration
    // out = a + b * factor
    void addState(const double a[4], const double b[4], double factor, double out[4]);

    // Perform one RK4 integration step
    // Updates ray state using 4th order Runge-Kutta method
    void rk4Step(Ray& ray, double dlambda);
}