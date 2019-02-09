
#include "math.h"

#define MAX_DISTANCE (1000000)

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

real sphere_sweep_triangle(sphere s, vector v, triangle t,
                           vector *touch_point) {
  plane p = triangle_to_plane(t);
  vector vn = vector_normalize(v);
  *touch_point = sphere_sweep_plane(s, vn, p);
  if (!point_in_triangle(*touch_point, t)) {
    *touch_point = closest_point_on_triangle(t.a, t.b, t.c, *touch_point);
    // print_vec(" > touch point edge", (*touch_point));
  } else {
    // print_vec(" > touch point tri", (*touch_point));
  }
  vector rv = vector_mul_real(vn, -1);
  // print_vec(" > bw ray", rv);
  return ray_intersect_sphere(*touch_point, rv, s.origin, s.radius);
  // return vector_add_vector(sphere_intersection_point, vector_mul_real(v, d));
}

int test_triangles(collision *collision, int num_triangles,
                   triangle *triangles) {

  vector point;
  real d;

  collision->collided = 0;
  collision->distance = MAX_DISTANCE;

  for (int i = 0; i < num_triangles; i++) {
    d = sphere_sweep_triangle(collision->object, collision->velocity,
                              triangles[i], &point);
    if (d >= 0 && d < collision->distance) {
      collision->distance = d;
      collision->point = point;
    }
  }
  collision->collided =
      vector_length(collision->velocity) >= collision->distance;
  return collision->collided;
}

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
