#ifndef BODYSPHERE_H
#define BODYSPHERE_H

#include "../../math/math.h"

#include "triangle.h"
#include "utlist.h"

#include "../../core/buffer.h"

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
#endif