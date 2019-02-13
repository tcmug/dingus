
#include "math.h"

TW_Vector TW_VectorAddVector(TW_Vector a, TW_Vector b) {
  return (TW_Vector){a.x + b.x, a.y + b.y, a.z + b.z};
}

TW_Vector TW_VectorAddReal(TW_Vector a, real b) {
  return (TW_Vector){a.x + b, a.y + b, a.z + b};
}

TW_Vector TW_VectorSubVector(TW_Vector a, TW_Vector b) {
  return (TW_Vector){a.x - b.x, a.y - b.y, a.z - b.z};
}

TW_Vector TW_VectorSubReal(TW_Vector a, real b) {
  return (TW_Vector){a.x - b, a.y - b, a.z - b};
}

TW_Vector TW_VectorMulVector(TW_Vector a, TW_Vector b) {
  return (TW_Vector){a.x * b.x, a.y * b.y, a.z * b.z};
}

TW_Vector TW_VectorMulReal(TW_Vector a, real b) {
  return (TW_Vector){a.x * b, a.y * b, a.z * b};
}

TW_Vector TW_VectorDivVector(TW_Vector a, TW_Vector b) {
  return (TW_Vector){a.x / b.x, a.y / b.y, a.z / b.z};
}

TW_Vector TW_VectorDivReal(TW_Vector a, real b) {
  return (TW_Vector){a.x / b, a.y / b, a.z / b};
}

real TW_VectorLengthSquared(TW_Vector a) {
  return (a.x * a.x) + (a.y * a.y) + (a.z * a.z);
}

real TW_VectorLength(TW_Vector a) { return sqrtf(TW_VectorLengthSquared(a)); }

TW_Vector TW_VectorNormalize(TW_Vector a) {
  return TW_VectorMulReal(a, 1.0 / TW_VectorLength(a));
}

real TW_VectorsDistance(TW_Vector a, TW_Vector b) {
  return TW_VectorLength(TW_VectorSubVector(b, a));
}

real TW_VectorsSquaredDistance(TW_Vector a, TW_Vector b) {
  return TW_VectorLengthSquared(TW_VectorSubVector(b, a));
}

real TW_VectorDot(TW_Vector a, TW_Vector b) {
  return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

TW_Vector TW_VectorCross(TW_Vector a, TW_Vector b) {
  return (TW_Vector){(a.y * b.z) - (a.z * b.y), (a.z * b.x) - (a.x * b.z),
                     (a.x * b.y) - (a.y * b.x)};
}

TW_Vector TW_VectorReflect(TW_Vector v, TW_Vector normal) {
  return TW_VectorAddVector(
      TW_VectorMulReal(normal, -2 * TW_VectorDot(v, normal)), v);
}
