
#include "math.h"

plane triangle_to_plane(triangle tri) {
  vector normal = vector_normalize(vector_cross(
      vector_sub_vector(tri.c, tri.a), vector_sub_vector(tri.b, tri.a)));
  return (plane){tri.a, normal, -vector_dot(tri.a, normal)};
}
