#ifndef DINGUS_TYPES_H
#define DINGUS_TYPES_H

#define real float

typedef struct s_point {
  union {
    struct {
      real x, y;
    };
    real value[2];
  };
} point;

typedef struct s_vector {
  union {
    struct {
      real x, y, z;
    };
    real value[3];
  };
} vector;

typedef struct s_matrix {
  union {
    struct {
      real a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p;
    };
    real value[16];
  };
} matrix;

typedef struct s_sphere {
  vector origin;
  real radius;
} sphere;

typedef struct s_plane {
  vector origin, normal;
  real normDistance;
} plane;

typedef struct s_triangle {
  vector a, b, c;
} triangle;

typedef struct s_collision {
  sphere object;
  vector velocity;

  int collided;
  real distance;
  vector point;
} collision;

#endif