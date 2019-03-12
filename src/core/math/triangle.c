
#include "math.h"

inline TW_Plane triangle_to_plane(TW_Triangle tri) {
  TW_Vector3 normal = TW_Vector3Normalize(TW_Vector3Cross(
      TW_Vector3SubVector(tri.c, tri.a), TW_Vector3SubVector(tri.b, tri.a)));
  return (TW_Plane){tri.a, normal, -TW_Vector3Dot(tri.a, normal)};
}
