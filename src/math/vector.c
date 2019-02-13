
#include "math.h"

vector vector_add_vector(vector a, vector b) {
  return (vector){a.x + b.x, a.y + b.y, a.z + b.z};
}

vector vector_add_real(vector a, real b) {
  return (vector){a.x + b, a.y + b, a.z + b};
}

vector vector_sub_vector(vector a, vector b) {
  return (vector){a.x - b.x, a.y - b.y, a.z - b.z};
}

vector vector_sub_real(vector a, real b) {
  return (vector){a.x - b, a.y - b, a.z - b};
}

vector vector_mul_vector(vector a, vector b) {
  return (vector){a.x * b.x, a.y * b.y, a.z * b.z};
}

vector vector_mul_real(vector a, real b) {
  return (vector){a.x * b, a.y * b, a.z * b};
}

vector vector_div_vector(vector a, vector b) {
  return (vector){a.x / b.x, a.y / b.y, a.z / b.z};
}

vector vector_div_real(vector a, real b) {
  return (vector){a.x / b, a.y / b, a.z / b};
}

real vector_length_squared(vector a) {
  return (a.x * a.x) + (a.y * a.y) + (a.z * a.z);
}

real vector_length(vector a) { return sqrtf(vector_length_squared(a)); }

vector vector_normalize(vector a) {
  return vector_mul_real(a, 1.0 / vector_length(a));
}

real distance_between_vectors(vector a, vector b) {
  return vector_length(vector_sub_vector(b, a));
}

real squared_distance_between_vectors(vector a, vector b) {
  return vector_length_squared(vector_sub_vector(b, a));
}

real vector_dot(vector a, vector b) {
  return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

vector vector_cross(vector a, vector b) {
  return (vector){(a.y * b.z) - (a.z * b.y), (a.z * b.x) - (a.x * b.z),
                  (a.x * b.y) - (a.y * b.x)};
}

vector vector_reflect(vector v, vector normal) {
  return vector_add_vector(vector_mul_real(normal, -2 * vector_dot(v, normal)),
                           v);
}
