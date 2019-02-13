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
} TW_Point;

typedef struct rectangle_t {
  union {
    struct {
      real x, y, w, h;
    };
    real value[4];
  };

} TW_Rectangle;

typedef struct vector_t {
  union {
    struct {
      real x, y, z;
    };
    real value[3];
  };
} TW_Vector;

typedef struct matrix_t {
  union {
    struct {
      real a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p;
    };
    real value[16];
  };
} TW_Matrix;

typedef struct sphere_t {
  TW_Vector origin;
  real radius;
} TW_Sphere;

typedef struct plane_t {
  TW_Vector origin, normal;
  real normDistance;
} TW_Plane;

typedef struct triangle_t {
  TW_Vector a, b, c;
} TW_Triangle;

typedef struct collision_t {
  TW_Sphere object;
  TW_Vector velocity;

  int collided;
  real distance;
  TW_Vector TW_Point;
} TW_Collision;

#endif