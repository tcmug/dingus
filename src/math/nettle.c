
#include "math.h"

#define MAX_DISTANCE (1000000)

// Inputs: TW_Plane origin, TW_Plane normal, ray origin ray TW_Vector3.
// NOTE: both vectors are assumed to be normalized
real TW_RayIntersectPlane(TW_Vector3 planeOrigin, TW_Vector3 planeNormal,
                          TW_Vector3 rayOrigin, TW_Vector3 rayVector) {
  real d = -(TW_Vector3Dot(planeNormal, planeOrigin));
  double numer = TW_Vector3Dot(planeNormal, rayOrigin) + d;
  double denom = TW_Vector3Dot(planeNormal, rayVector);
  return -(numer / denom);
}

real TW_RayIntersectSphere(TW_Vector3 rO, TW_Vector3 rV, TW_Vector3 sO,
                           real sR) {
  TW_Vector3 Q = TW_Vector3SubVector(sO, rO);
  real c = TW_Vector3Length(Q);
  real v = TW_Vector3Dot(Q, rV);
  real d = sR * sR - (c * c - v * v);
  // If there was no intersection, return -1
  if (d < 0.0)
    return -1.0;
  // Return the distance to the [first] intersecting TW_Vector2
  return v - sqrt(d);
}

TW_Vector3 TW_SphereSweepPlane(TW_Sphere s, TW_Vector3 v, TW_Plane p) {
  TW_Vector3 sphere_intersection_point =
      TW_Vector3AddVector(s.origin, TW_Vector3MulReal(p.normal, -s.radius));
  real d = TW_RayIntersectPlane(p.origin, p.normal, sphere_intersection_point,
                                TW_Vector3Normalize(v));
  return TW_Vector3AddVector(sphere_intersection_point,
                             TW_Vector3MulReal(v, d));
}

TW_Vector3 TW_ClosestPointOnPlane(TW_Vector3 a, TW_Vector3 b, TW_Vector3 p) {
  // Determine t (the length of the TW_Vector3 from ‘a’ to ‘p’)
  TW_Vector3 c = TW_Vector3SubVector(p, a);
  TW_Vector3 V = TW_Vector3Normalize(TW_Vector3SubVector(b, a));
  real d = TW_Vector3Length(TW_Vector3SubVector(b, a));
  real t = TW_Vector3Dot(V, c);
  // Check to see if ‘t’ is beyond the extents of the line segment
  if (t < 0)
    return a;
  if (t > d)
    return b;
  // Return the TW_Vector2 between ‘a’ and ‘b’
  return TW_Vector3AddVector(a, TW_Vector3MulReal(V, t));
  /*
  Vector c = p - a;
  VectorV = Normalized TW_Vector3[b –a];
  double d = distance from a to b;
  double t = V * c;
  // Check to see if ‘t’ is beyond the extents of the line segment
  if (t < 0)
    return a;
  if (t > d)
    return b;
  // Return the TW_Vector2 between ‘a’ and ‘b’
  // set length of V to t;
  */
}

TW_Vector3 TW_ClosestPointOnTriangle(TW_Vector3 a, TW_Vector3 b, TW_Vector3 c,
                                     TW_Vector3 p) {
  TW_Vector3 Rab = TW_ClosestPointOnPlane(a, b, p);
  TW_Vector3 Rbc = TW_ClosestPointOnPlane(b, c, p);
  TW_Vector3 Rca = TW_ClosestPointOnPlane(c, a, p);
  real Rabl = TW_Vector3sSquaredDistance(Rab, p);
  real Rbcl = TW_Vector3sSquaredDistance(Rbc, p);
  real Rcal = TW_Vector3sSquaredDistance(Rca, p);
  if (Rabl <= Rbcl && Rabl <= Rcal) {
    return Rab;
  } else if (Rbcl <= Rabl && Rbcl <= Rcal) {
    return Rbc;
  }
  // else if (Rcal <= Rabl && Rcal <= Rbcl) {
  return Rca;
}

real TW_SphereSweepTriangle(TW_Sphere s, TW_Vector3 v, TW_Triangle t,
                            TW_Vector3 *touch_point) {
  TW_Plane p = triangle_to_plane(t);
  TW_Vector3 vn = TW_Vector3Normalize(v);
  *touch_point = TW_SphereSweepPlane(s, vn, p);
  if (!TW_Vector2InTriangle(*touch_point, t)) {
    *touch_point = TW_ClosestPointOnTriangle(t.a, t.b, t.c, *touch_point);
    // print_vec(" > touch TW_Vector2 edge", (*touch_point));
  } else {
    // print_vec(" > touch TW_Vector2 tri", (*touch_point));
  }
  TW_Vector3 rv = TW_Vector3MulReal(vn, -1);
  // print_vec(" > bw ray", rv);
  return TW_RayIntersectSphere(*touch_point, rv, s.origin, s.radius);
  // return TW_Vector3AddVector(sphere_intersection_point, TW_Vector3MulReal(v,
  // d));
}

int TW_TestTriangles(TW_Collision *TW_Collision, int num_triangles,
                     TW_Triangle *triangles) {

  TW_Vector3 TW_Vector2;
  real d;

  TW_Collision->collided = 0;
  TW_Collision->distance = MAX_DISTANCE;

  for (int i = 0; i < num_triangles; i++) {
    d = TW_SphereSweepTriangle(TW_Collision->object, TW_Collision->velocity,
                               triangles[i], &TW_Vector2);
    if (d >= 0 && d < TW_Collision->distance) {
      TW_Collision->distance = d;
      TW_Collision->TW_Vector2 = TW_Vector2;
    }
  }
  TW_Collision->collided =
      TW_Vector3Length(TW_Collision->velocity) >= TW_Collision->distance;
  return TW_Collision->collided;
}

int TW_Vector2InTriangle(TW_Vector3 TW_Vector2, TW_Triangle t) {
  // u=P2−P1
  TW_Vector3 u = TW_Vector3SubVector(t.b, t.a);
  // v=P3−P1
  TW_Vector3 v = TW_Vector3SubVector(t.c, t.a);
  // n=u×v
  TW_Vector3 n = TW_Vector3Cross(u, v);
  // w=P−P1
  TW_Vector3 w = TW_Vector3SubVector(TW_Vector2, t.a);
  // Barycentric coordinates of the projection P′of P onto T:
  real n2 = TW_Vector3Dot(n, n);
  // γ=[(u×w)⋅n]/n²
  real gamma = TW_Vector3Dot(TW_Vector3Cross(u, w), n) / n2;
  // β=[(w×v)⋅n]/n²
  real beta = TW_Vector3Dot(TW_Vector3Cross(w, v), n) / n2;
  real alpha = 1 - gamma - beta;
  // The TW_Vector2 P′ lies inside T if:
  return ((0 <= alpha) && (alpha <= 1) && (0 <= beta) && (beta <= 1) &&
          (0 <= gamma) && (gamma <= 1));
}
