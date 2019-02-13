
#include "math.h"

inline int TW_IsPlaneFacingDirection(TW_Plane p, TW_Vector d) {
  return TW_VectorDot(p.normal, d) <= 0;
}

inline real TW_VectorDistanceToPlane(TW_Vector a, TW_Plane p) {
  return TW_VectorDot(a, p.normal) + p.normDistance;
}
