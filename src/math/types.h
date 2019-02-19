#ifndef DINGUS_TYPES_H
#define DINGUS_TYPES_H

#define real float

typedef struct rectangle_t {
  union {
    struct {
      real x, y, w, h;
    };
    real value[4];
  };

} TW_Rectangle;

typedef struct TW_Vecto2_t {
  union {
    struct {
      real x, y;
    };
    real value[2];
  };
} TW_Vector2;

typedef struct TW_Vector3_t {
  union {
    struct {
      real x, y, z;
    };
    real value[3];
  };
} TW_Vector3;

typedef struct TW_Vector4_t {
  union {
    struct {
      real x, y, z, w;
    };
    real value[4];
  };
} TW_Vector4;

typedef struct matrix_t {
  union {
    struct {
      real a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p;
    };
    real value[16];
  };
} TW_Matrix;

typedef struct sphere_t {
  TW_Vector3 origin;
  real radius;
} TW_Sphere;

typedef struct plane_t {
  TW_Vector3 origin, normal;
  real normDistance;
} TW_Plane;

typedef struct triangle_t {
  TW_Vector3 a, b, c;
} TW_Triangle;

typedef struct collision_t {
  TW_Sphere object;
  TW_Vector3 velocity;

  int collided;
  real distance;
  TW_Vector3 TW_Vector2;
} TW_Collision;

#endif