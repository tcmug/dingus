#ifndef Sphere_h
#define Sphere_h

#include "../../core/buffer.h"
#include "../../core/math.h"

#include "utlist.h"

typedef struct s_bodytri {
  TW_Vector3 points[3];
  TW_Vector3 mesh[12];
  TW_Vector3 normal;
  int type;
  struct s_bodytri *parent;
  struct s_bodytri *children;
  struct s_bodytri *prev, *next;
} bodytri;

typedef struct s_bodysphere {
  TW_Vector3Buffer *render_triangles;
  TW_Vector2Buffer *render_uvs;
  bodytri *triangles;
  bodytri *priority;
  bodytri *merge;
  TW_Vector3 position;
  real radius;
  TW_Matrix matrix;
} bodysphere;

void bodysphere_create(bodysphere *sphere, real radius);
void bodysphere_update(bodysphere *sphere, TW_Vector3 viewer);
void bodysphere_render(bodysphere *sphere);

void triangle_split(TW_Vector3 *abc, TW_Vector3 *result);
void bodytri_create(bodytri *tri, TW_Vector3 points[]);
void bodytri_split(bodysphere *sphere, bodytri *tri);
void bodytri_merge(bodysphere *sphere, bodytri *tri);

#endif