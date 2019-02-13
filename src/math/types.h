#ifndef DINGUS_TYPES_H
#define DINGUS_TYPES_H

#define real float

typedef struct point_t {
  union {
    struct {
      real x, y;
    };
    real value[2];
  };
} point;

typedef struct rectangle_t {
  union {
    struct {
      real x, y, w, h;
    };
    real value[4];
  };

} rectangle;

typedef struct vector_t {
  union {
    struct {
      real x, y, z;
    };
    real value[3];
  };
} vector;

typedef struct matrix_t {
  union {
    struct {
      real a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p;
    };
    real value[16];
  };
} matrix;

typedef struct sphere_t {
  vector origin;
  real radius;
} sphere;

typedef struct plane_t {
  vector origin, normal;
  real normDistance;
} plane;

typedef struct triangle_t {
  vector a, b, c;
} triangle;

typedef struct collision_t {
  sphere object;
  vector velocity;

  int collided;
  real distance;
  vector point;
} collision;

#endif