#ifndef DINGUS_FUNCTIONS_H
#define DINGUS_FUNCTIONS_H

#include "math.h"

#define print_vec(str, vec) printf("%s: %f %f %f\n", str, vec.x, vec.y, vec.z)

TW_Vector TW_VectorAddVector(TW_Vector a, TW_Vector b);
TW_Vector TW_VectorAddReal(TW_Vector a, real b);
TW_Vector TW_VectorSubVector(TW_Vector a, TW_Vector b);
TW_Vector TW_VectorSubReal(TW_Vector a, real b);
TW_Vector TW_VectorMulVector(TW_Vector a, TW_Vector b);
TW_Vector TW_VectorMulReal(TW_Vector a, real b);
TW_Vector TW_VectorDivVector(TW_Vector a, TW_Vector b);
TW_Vector TW_VectorDivReal(TW_Vector a, real b);
real TW_VectorLengthSquared(TW_Vector a);
real TW_VectorLength(TW_Vector a);
TW_Vector TW_VectorNormalize(TW_Vector a);
real TW_VectorsDistance(TW_Vector a, TW_Vector b);
real TW_VectorsSquaredDistance(TW_Vector a, TW_Vector b);
real TW_VectorDot(TW_Vector a, TW_Vector b);
TW_Vector TW_VectorCross(TW_Vector a, TW_Vector b);
TW_Vector TW_VectorReflect(TW_Vector v, TW_Vector normal);

int TW_RectangleIncludesPoint(TW_Rectangle r, TW_Point p);

TW_Matrix TW_MatrixIdentity();
TW_Matrix TW_MatrixTransposed(TW_Matrix m);
TW_Vector TW_MatrixMulVector(TW_Matrix m, TW_Vector v);
TW_Matrix TW_MatrixTranslation(TW_Matrix m, TW_Vector v);
TW_Matrix TW_MatrixScaling(TW_Matrix m, TW_Vector v);
TW_Vector TW_MatrixProjectVector(TW_Matrix m, TW_Vector v);
TW_Matrix TW_MatrixMulMatrix(TW_Matrix a, TW_Matrix b);
void TW_MatrixGLUniform(const char *name, TW_Matrix m);
TW_Matrix TW_MatrixFromVector(TW_Vector eye, TW_Vector to, TW_Vector up);
TW_Matrix TW_MatrixPerspectiveProjection(real near, real far, real fov,
                                         real ratio);
TW_Matrix TW_MatrixRotation(real psi, real theta, real phi);
TW_Matrix TW_MatrixOrthogonalProjection(real left, real right, real bottom,
                                        real top, real near, real far);

int TW_IsPlaneFacingDirection(TW_Plane p, TW_Vector d);
real TW_VectorDistanceToPlane(TW_Vector a, TW_Plane p);

real TW_SphereDistanceToPlane(TW_Sphere s, TW_Plane p);

TW_Plane triangle_to_plane(TW_Triangle tri);

real TW_RayIntersectPlane(TW_Vector planeOrigin, TW_Vector planeNormal,
                          TW_Vector rayOrigin, TW_Vector rayVector);
real TW_RayIntersectSphere(TW_Vector rO, TW_Vector rV, TW_Vector sO, real sR);
TW_Vector TW_SphereSweepPlane(TW_Sphere s, TW_Vector v, TW_Plane p);
TW_Vector TW_ClosestPointOnPlane(TW_Vector a, TW_Vector b, TW_Vector p);
TW_Vector TW_ClosestPointOnTriangle(TW_Vector a, TW_Vector b, TW_Vector c,
                                    TW_Vector p);
real TW_SphereSweepTriangle(TW_Sphere s, TW_Vector v, TW_Triangle t,
                            TW_Vector *touch_point);
int TW_TestTriangles(TW_Collision *TW_Collision, int num_triangles,
                     TW_Triangle *triangles);
int TW_PointInTriangle(TW_Vector TW_Point, TW_Triangle t);

#endif