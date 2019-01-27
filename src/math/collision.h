#ifndef DINGUS_COLLISION_H
#define DINGUS_COLLISION_H

#include "math.h"

int point_in_triangle(vector point, triangle t) {
  // u=P2−P1
  vector u = vector_sub_vector(t.b, t.a);
  // v=P3−P1
  vector v = vector_sub_vector(t.c, t.a);
  // n=u×v
  vector n = vector_cross(u, v);
  // w=P−P1
  vector w = vector_sub_vector(point, t.a);
  // Barycentric coordinates of the projection P′of P onto T:
  real n2 = vector_dot(n, n);
  // γ=[(u×w)⋅n]/n²
  real gamma = vector_dot(vector_cross(u, w), n) / n2;
  // β=[(w×v)⋅n]/n²
  real beta = vector_dot(vector_cross(w, v), n) / n2;
  real alpha = 1 - gamma - beta;
  // The point P′ lies inside T if:
  return ((0 <= alpha) && (alpha <= 1) && (0 <= beta) && (beta <= 1) &&
          (0 <= gamma) && (gamma <= 1));
}

real sphere_triangle_sweep(sphere s, vector v, triangle t) {}

#endif