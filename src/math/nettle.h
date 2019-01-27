#ifndef NETTLE_H
#define NETTLE_H

#include "math.h"

// Inputs: plane origin, plane normal, ray origin ray vector.
// NOTE: both vectors are assumed to be normalized
real ray_intersect_plane(vector planeOrigin, vector planeNormal,
                         vector rayOrigin, vector rayVector) {
  real d = -(vector_dot(planeNormal, planeOrigin));
  double numer = vector_dot(planeNormal, rayOrigin) + d;
  double denom = vector_dot(planeNormal, rayVector);
  return -(numer / denom);
}

real ray_intersect_sphere(vector rO, vector rV, vector sO, real sR) {
  vector Q = vector_sub_vector(sO, rO);
  real c = vector_length(Q);
  real v = vector_dot(Q, rV);
  real d = sR * sR - (c * c - v * v);
  // If there was no intersection, return -1
  if (d < 0.0)
    return -1.0;
  // Return the distance to the [first] intersecting point
  return v - sqrt(d);
}

vector sphere_sweep_plane(sphere s, vector v, plane p) {
  vector sphere_intersection_point =
      vector_add_vector(s.origin, vector_mul_real(p.normal, -s.radius));
  real d = ray_intersect_plane(p.origin, p.normal, sphere_intersection_point,
                               vector_normalize(v));
  return vector_add_vector(sphere_intersection_point, vector_mul_real(v, d));
}

vector closest_point_on_line(vector a, vector b, vector p) {
  // Determine t (the length of the vector from ‘a’ to ‘p’)
  vector c = vector_sub_vector(p, a);
  vector V = vector_normalize(vector_sub_vector(b, a));
  real d = vector_length(vector_sub_vector(b, a));
  real t = vector_dot(V, c);
  // Check to see if ‘t’ is beyond the extents of the line segment
  if (t < 0)
    return a;
  if (t > d)
    return b;
  // Return the point between ‘a’ and ‘b’
  return vector_add_vector(a, vector_mul_real(V, t));
  /*
  Vector c = p - a;
  VectorV = Normalized vector[b –a];
  double d = distance from a to b;
  double t = V * c;
  // Check to see if ‘t’ is beyond the extents of the line segment
  if (t < 0)
    return a;
  if (t > d)
    return b;
  // Return the point between ‘a’ and ‘b’
  // set length of V to t;
  */
}

vector closest_point_on_triangle(vector a, vector b, vector c, vector p) {
  vector Rab = closest_point_on_line(a, b, p);
  vector Rbc = closest_point_on_line(b, c, p);
  vector Rca = closest_point_on_line(c, a, p);
  real Rabl = squared_distance_between_vectors(Rab, p);
  real Rbcl = squared_distance_between_vectors(Rbc, p);
  real Rcal = squared_distance_between_vectors(Rca, p);
  if (Rabl <= Rbcl && Rabl <= Rcal) {
    return Rab;
  } else if (Rbcl <= Rabl && Rbcl <= Rcal) {
    return Rbc;
  }
  // else if (Rcal <= Rabl && Rcal <= Rbcl) {
  return Rca;
}

real sphere_sweep_triangle(sphere s, vector v, triangle t) {
  plane p = triangle_to_plane(t);
  vector vn = vector_normalize(v);
  vector touch_point = sphere_sweep_plane(s, vn, p);
  if (!point_in_triangle(touch_point, t)) {
    touch_point = closest_point_on_triangle(t.a, t.b, t.c, touch_point);
    // print_vec(" > touch point edge", touch_point);
  } else {
    // print_vec(" > touch point tri", touch_point);
  }
  vector rv = vector_mul_real(vn, -1);
  // print_vec(" > bw ray", rv);
  return ray_intersect_sphere(touch_point, rv, s.origin, s.radius);
  // return vector_add_vector(sphere_intersection_point, vector_mul_real(v, d));
}

#endif