
#include "math.h"

#define MAX_DISTANCE (1000000)

// Inputs: TW_Plane origin, TW_Plane normal, ray origin ray TW_Vector.
// NOTE: both vectors are assumed to be normalized
real TW_RayIntersectPlane(TW_Vector planeOrigin, TW_Vector planeNormal,
                          TW_Vector rayOrigin, TW_Vector rayVector) {
  real d = -(TW_VectorDot(planeNormal, planeOrigin));
  double numer = TW_VectorDot(planeNormal, rayOrigin) + d;
  double denom = TW_VectorDot(planeNormal, rayVector);
  return -(numer / denom);
}

real TW_RayIntersectSphere(TW_Vector rO, TW_Vector rV, TW_Vector sO, real sR) {
  TW_Vector Q = TW_VectorSubVector(sO, rO);
  real c = TW_VectorLength(Q);
  real v = TW_VectorDot(Q, rV);
  real d = sR * sR - (c * c - v * v);
  // If there was no intersection, return -1
  if (d < 0.0)
    return -1.0;
  // Return the distance to the [first] intersecting TW_Point
  return v - sqrt(d);
}

TW_Vector TW_SphereSweepPlane(TW_Sphere s, TW_Vector v, TW_Plane p) {
  TW_Vector sphere_intersection_point =
      TW_VectorAddVector(s.origin, TW_VectorMulReal(p.normal, -s.radius));
  real d = TW_RayIntersectPlane(p.origin, p.normal, sphere_intersection_point,
                                TW_VectorNormalize(v));
  return TW_VectorAddVector(sphere_intersection_point, TW_VectorMulReal(v, d));
}

TW_Vector TW_ClosestPointOnPlane(TW_Vector a, TW_Vector b, TW_Vector p) {
  // Determine t (the length of the TW_Vector from ‘a’ to ‘p’)
  TW_Vector c = TW_VectorSubVector(p, a);
  TW_Vector V = TW_VectorNormalize(TW_VectorSubVector(b, a));
  real d = TW_VectorLength(TW_VectorSubVector(b, a));
  real t = TW_VectorDot(V, c);
  // Check to see if ‘t’ is beyond the extents of the line segment
  if (t < 0)
    return a;
  if (t > d)
    return b;
  // Return the TW_Point between ‘a’ and ‘b’
  return TW_VectorAddVector(a, TW_VectorMulReal(V, t));
  /*
  Vector c = p - a;
  VectorV = Normalized TW_Vector[b –a];
  double d = distance from a to b;
  double t = V * c;
  // Check to see if ‘t’ is beyond the extents of the line segment
  if (t < 0)
    return a;
  if (t > d)
    return b;
  // Return the TW_Point between ‘a’ and ‘b’
  // set length of V to t;
  */
}

TW_Vector TW_ClosestPointOnTriangle(TW_Vector a, TW_Vector b, TW_Vector c,
                                    TW_Vector p) {
  TW_Vector Rab = TW_ClosestPointOnPlane(a, b, p);
  TW_Vector Rbc = TW_ClosestPointOnPlane(b, c, p);
  TW_Vector Rca = TW_ClosestPointOnPlane(c, a, p);
  real Rabl = TW_VectorsSquaredDistance(Rab, p);
  real Rbcl = TW_VectorsSquaredDistance(Rbc, p);
  real Rcal = TW_VectorsSquaredDistance(Rca, p);
  if (Rabl <= Rbcl && Rabl <= Rcal) {
    return Rab;
  } else if (Rbcl <= Rabl && Rbcl <= Rcal) {
    return Rbc;
  }
  // else if (Rcal <= Rabl && Rcal <= Rbcl) {
  return Rca;
}

real TW_SphereSweepTriangle(TW_Sphere s, TW_Vector v, TW_Triangle t,
                            TW_Vector *touch_point) {
  TW_Plane p = triangle_to_plane(t);
  TW_Vector vn = TW_VectorNormalize(v);
  *touch_point = TW_SphereSweepPlane(s, vn, p);
  if (!TW_PointInTriangle(*touch_point, t)) {
    *touch_point = TW_ClosestPointOnTriangle(t.a, t.b, t.c, *touch_point);
    // print_vec(" > touch TW_Point edge", (*touch_point));
  } else {
    // print_vec(" > touch TW_Point tri", (*touch_point));
  }
  TW_Vector rv = TW_VectorMulReal(vn, -1);
  // print_vec(" > bw ray", rv);
  return TW_RayIntersectSphere(*touch_point, rv, s.origin, s.radius);
  // return TW_VectorAddVector(sphere_intersection_point, TW_VectorMulReal(v,
  // d));
}

int TW_TestTriangles(TW_Collision *TW_Collision, int num_triangles,
                     TW_Triangle *triangles) {

  TW_Vector TW_Point;
  real d;

  TW_Collision->collided = 0;
  TW_Collision->distance = MAX_DISTANCE;

  for (int i = 0; i < num_triangles; i++) {
    d = TW_SphereSweepTriangle(TW_Collision->object, TW_Collision->velocity,
                               triangles[i], &TW_Point);
    if (d >= 0 && d < TW_Collision->distance) {
      TW_Collision->distance = d;
      TW_Collision->TW_Point = TW_Point;
    }
  }
  TW_Collision->collided =
      TW_VectorLength(TW_Collision->velocity) >= TW_Collision->distance;
  return TW_Collision->collided;
}

int TW_PointInTriangle(TW_Vector TW_Point, TW_Triangle t) {
  // u=P2−P1
  TW_Vector u = TW_VectorSubVector(t.b, t.a);
  // v=P3−P1
  TW_Vector v = TW_VectorSubVector(t.c, t.a);
  // n=u×v
  TW_Vector n = TW_VectorCross(u, v);
  // w=P−P1
  TW_Vector w = TW_VectorSubVector(TW_Point, t.a);
  // Barycentric coordinates of the projection P′of P onto T:
  real n2 = TW_VectorDot(n, n);
  // γ=[(u×w)⋅n]/n²
  real gamma = TW_VectorDot(TW_VectorCross(u, w), n) / n2;
  // β=[(w×v)⋅n]/n²
  real beta = TW_VectorDot(TW_VectorCross(w, v), n) / n2;
  real alpha = 1 - gamma - beta;
  // The TW_Point P′ lies inside T if:
  return ((0 <= alpha) && (alpha <= 1) && (0 <= beta) && (beta <= 1) &&
          (0 <= gamma) && (gamma <= 1));
}
