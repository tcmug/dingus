
#include "math.h"

inline TW_Plane triangle_to_plane(TW_Triangle tri) {
  TW_Vector normal = TW_VectorNormalize(TW_VectorCross(
      TW_VectorSubVector(tri.c, tri.a), TW_VectorSubVector(tri.b, tri.a)));
  return (TW_Plane){tri.a, normal, -TW_VectorDot(tri.a, normal)};
}
