
#include "math.h"

inline int TW_IsPlaneFacingDirection(TW_Plane p, TW_Vector3 d) {
  return TW_Vector3Dot(p.normal, d) <= 0;
}

inline real TW_Vector3DistanceToPlane(TW_Vector3 a, TW_Plane p) {
  return TW_Vector3Dot(a, p.normal) + p.normDistance;
}
