
#include "math.h"

inline int plane_facing_direction(plane p, vector d) {
  return vector_dot(p.normal, d) <= 0;
}

inline real vector_distance_to_plane(vector a, plane p) {
  return vector_dot(a, p.normal) + p.normDistance;
}
