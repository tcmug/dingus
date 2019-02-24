#ifndef BODYTRIANGLE_H
#define BODYTRIANGLE_H

#include "../../math/math.h";
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

#endif