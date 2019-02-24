#ifndef BODYSPHERE_H
#define BODYSPHERE_H

#include "../../math/math.h"

#include "triangle.h"
#include "utlist.h"

typedef struct s_bodysphere {
  bodytri *triangles;
  bodytri *priority;
  bodytri *merge;
  TW_Vector3 position;
  real radius;
  TW_Matrix matrix;
} bodysphere;

void bodysphere_create(bodysphere *sphere, real radius);
void bodysphere_update(bodysphere *sphere, TW_Vector3 viewer);

#endif