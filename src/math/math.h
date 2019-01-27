#ifndef DINGUS_MATH_H
#define DINGUS_MATH_H

#include <math.h>

#define real float

typedef struct s_vector {
  union {
    struct {
      real x, y, z;
    };
    real value[3];
  };
} vector;

#define print_vec(str, vec) printf("%s: %f %f %f\n", str, vec.x, vec.y, vec.z)

typedef struct s_triangle {
  vector a, b, c;
} triangle;

typedef struct s_plane {
  vector origin, normal;
  real normDistance;
} plane;

typedef struct s_sphere {
  vector origin;
  real radius;
} sphere;

typedef struct s_matrix {
  union {
    struct {
      real a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p;
    };
    real value[4 * 4];
  };
} matrix;

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

plane triangle_to_plane(triangle tri) {
  vector normal = vector_normalize(vector_cross(
      vector_sub_vector(tri.c, tri.a), vector_sub_vector(tri.b, tri.a)));
  return (plane){tri.a, normal, -vector_dot(tri.a, normal)};
}

int plane_facing_direction(plane p, vector d) {
  return vector_dot(p.normal, d) <= 0;
}

int get_lowest_root(float a, float b, float c, float maxR, float *root) {
  // Check if a solution exists
  float determinant = b * b - 4.0f * a * c;
  // If determinant is negative it means no solutions.
  if (determinant < 0.0f)
    return 0;
  // calculate the two roots: (if determinant == 0 then
  // x1==x2 but let’s disregard that slight optimization)
  float sqrtD = sqrt(determinant);
  float r1 = (-b - sqrtD) / (2 * a);
  float r2 = (-b + sqrtD) / (2 * a);
  // Sort so x1 <= x2
  if (r1 > r2) {
    float temp = r2;
    r2 = r1;
    r1 = temp;
  }
  // Get lowest root:
  if (r1 > 0 && r1 < maxR) {
    *root = r1;
    return 1;
  }
  // It is possible that we want x2 - this can happen
  // if x1 < 0
  if (r2 > 0 && r2 < maxR) {
    *root = r2;
    return 1;
  }
  // No (valid) solutions
  return 0;
}

// real vector_distance_to_plane(vector a, plane p) {
//   return vector_dot(vector_sub_vector(a, p.origin), p.normal);
// }

real vector_distance_to_plane(vector a, plane p) {
  return vector_dot(a, p.normal) + p.normDistance;
}

real sphere_distance_to_plane(sphere s, plane p) {}

#include "collision.h"

#endif