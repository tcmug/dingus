#ifndef DINGUS_FUNCTIONS_H
#define DINGUS_FUNCTIONS_H

#include "math.h"

#define print_vec(str, vec) printf("%s: %f %f %f\n", str, vec.x, vec.y, vec.z)

TW_Vector3 TW_Vector3AddVector(TW_Vector3 a, TW_Vector3 b);
TW_Vector3 TW_Vector3AddReal(TW_Vector3 a, real b);
TW_Vector3 TW_Vector3SubVector(TW_Vector3 a, TW_Vector3 b);
TW_Vector3 TW_Vector3SubReal(TW_Vector3 a, real b);
TW_Vector3 TW_Vector3MulVector(TW_Vector3 a, TW_Vector3 b);
TW_Vector3 TW_Vector3MulReal(TW_Vector3 a, real b);
TW_Vector3 TW_Vector3DivVector(TW_Vector3 a, TW_Vector3 b);
TW_Vector3 TW_Vector3DivReal(TW_Vector3 a, real b);
real TW_Vector3LengthSquared(TW_Vector3 a);
real TW_Vector3Length(TW_Vector3 a);
TW_Vector3 TW_Vector3Normalize(TW_Vector3 a);
real TW_Vector3sDistance(TW_Vector3 a, TW_Vector3 b);
real TW_Vector3sSquaredDistance(TW_Vector3 a, TW_Vector3 b);
real TW_Vector3Dot(TW_Vector3 a, TW_Vector3 b);
TW_Vector3 TW_Vector3Cross(TW_Vector3 a, TW_Vector3 b);
TW_Vector3 TW_Vector3Reflect(TW_Vector3 v, TW_Vector3 normal);

int TW_RectangleIncludesPoint(TW_Rectangle r, TW_Vector2 p);

TW_Matrix TW_MatrixIdentity();
TW_Matrix TW_MatrixTransposed(TW_Matrix m);
TW_Vector3 TW_MatrixMulVector(TW_Matrix m, TW_Vector3 v);
TW_Matrix TW_MatrixTranslation(TW_Matrix m, TW_Vector3 v);
TW_Matrix TW_MatrixScaling(TW_Matrix m, TW_Vector3 v);
TW_Vector3 TW_MatrixProjectVector(TW_Matrix m, TW_Vector3 v);
TW_Matrix TW_MatrixMulMatrix(TW_Matrix a, TW_Matrix b);
void TW_MatrixGLUniform(const char *name, TW_Matrix m);
TW_Matrix TW_MatrixFromVector(TW_Vector3 eye, TW_Vector3 to, TW_Vector3 up);
TW_Matrix TW_MatrixPerspectiveProjection(real near, real far, real fov,
                                         real ratio);
TW_Matrix TW_MatrixRotation(real psi, real theta, real phi);
TW_Matrix TW_MatrixOrthogonalProjection(real left, real right, real bottom,
                                        real top, real near, real far);

int TW_IsPlaneFacingDirection(TW_Plane p, TW_Vector3 d);
real TW_Vector3DistanceToPlane(TW_Vector3 a, TW_Plane p);

real TW_SphereDistanceToPlane(TW_Sphere s, TW_Plane p);

TW_Plane triangle_to_plane(TW_Triangle tri);

real TW_RayIntersectPlane(TW_Vector3 planeOrigin, TW_Vector3 planeNormal,
                          TW_Vector3 rayOrigin, TW_Vector3 rayVector);
real TW_RayIntersectSphere(TW_Vector3 rO, TW_Vector3 rV, TW_Vector3 sO,
                           real sR);
TW_Vector3 TW_SphereSweepPlane(TW_Sphere s, TW_Vector3 v, TW_Plane p);
TW_Vector3 TW_ClosestPointOnPlane(TW_Vector3 a, TW_Vector3 b, TW_Vector3 p);
TW_Vector3 TW_ClosestPointOnTriangle(TW_Vector3 a, TW_Vector3 b, TW_Vector3 c,
                                     TW_Vector3 p);
real TW_SphereSweepTriangle(TW_Sphere s, TW_Vector3 v, TW_Triangle t,
                            TW_Vector3 *touch_point);
int TW_TestTriangles(TW_Collision *TW_Collision, int num_triangles,
                     TW_Triangle *triangles);
int TW_Vector2InTriangle(TW_Vector3 TW_Vector2, TW_Triangle t);

#endif