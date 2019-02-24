
#include "triangle.h"
#include "sphere.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TOP (0)
#define FRONTLEFT (1)
#define FRONTRIGHT (2)
#define BACKLEFT (3)
#define BACKRIGHT (4)
#define BOTTOM (5)

#define NORM1 (0.70710678118)

void triangle_split(TW_Vector3 *abc, TW_Vector3 *result) {
  TW_Vector3 points[6] = {
      UNFOLD(abc[0]), UNFOLD(abc[1]), UNFOLD(abc[2]), {0}, {0}, {0}};

  vec3_midpoint(points[3], points[0], points[2]);
  vec3_midpoint(points[4], points[0], points[1]);
  vec3_midpoint(points[5], points[1], points[2]);

  int i = 0;
  memcpy(result[i++], POINTS(points[0], points[4], points[3]),
         sizeof(TW_Vector3) * 3);
  memcpy(result[i++], POINTS(points[3], points[5], points[2]),
         sizeof(TW_Vector3) * 3);
  memcpy(result[i++], POINTS(points[4], points[1], points[5]),
         sizeof(TW_Vector3) * 3);
  memcpy(result[i++], POINTS(points[3], points[4], points[5]),
         sizeof(TW_Vector3) * 3);
}

void bodytri_create(bodytri *tri, TW_Vector3 points[]) {
  tri->children = 0;
  tri->type = 0;
  tri->parent = 0;
  tri->next = 0;
  tri->prev = 0;

  memcpy(tri->points, points, sizeof(TW_Vector3) * 3);

  point3_add(tri->normal, points[0], points[1]);
  point3_add(tri->normal, tri->normal, points[2]);
  point3_norm(tri->normal, tri->normal);

  // 0 is origin (0,0,0), 1 and 2 relative to that
  TW_Vector3 ms[3] = {0};
  point3_sub(ms[1], points[1], points[0]);
  point3_sub(ms[2], points[2], points[0]);

  TW_Vector3 to_split[3 * 4];

  for (int j = 0; j < 3; j++) {
    for (int i = 0; i < 3; i++) {
      tri->mesh[j][i] = ms[j][i];
    }
  }
}

void bodytri_split(bodysphere *sphere, bodytri *tri) {
  tri->children = malloc(4 * sizeof(bodytri));

  TW_Vector3 points[6] = {UNFOLD(tri->points[0]),
                          UNFOLD(tri->points[1]),
                          UNFOLD(tri->points[2]),
                          {0},
                          {0},
                          {0}};

  point3_midpoint(points[3], points[0], points[2]);
  point3_midpoint(points[4], points[0], points[1]);
  point3_midpoint(points[5], points[1], points[2]);

  for (int i = 0; i < 6; i++) {
    point3_norm(points[i], points[i]);

    real scale = 0;
    point3_scale(points[i], points[i], scale + sphere->radius);
  }
  /**
   *
   *      0
   *
   *    3   4
   *
   *  2   5    1
   *
   *
   **/

  int i = 0;
  bodytri_create(&tri->children[i++], POINTS(points[0], points[4], points[3]));
  bodytri_create(&tri->children[i++], POINTS(points[3], points[5], points[2]));
  bodytri_create(&tri->children[i++], POINTS(points[4], points[1], points[5]));
  bodytri_create(&tri->children[i++], POINTS(points[3], points[4], points[5]));

  // Move this triangle from the priority to the merge list and append children
  // to the priority list.
  if (tri->parent && tri->parent->type == 1) {
    DL_DELETE(sphere->merge, tri->parent);
    tri->parent->type = 0;
  }
  DL_DELETE(sphere->priority, tri);
  DL_APPEND(sphere->merge, tri);
  tri->type = 1;

  for (i = 0; i < 4; i++) {
    tri->children[i].parent = tri;
    DL_APPEND(sphere->priority, &tri->children[i]);
  }
}

void bodytri_merge(bodysphere *sphere, bodytri *tri) {

  assert(tri->type == 1);

  for (int i = 0; i < 4; i++) {
    assert(tri->children[i].children == 0);
  }

  for (int i = 0; i < 4; i++) {
    DL_DELETE(sphere->priority, &tri->children[i]);
  }

  free(tri->children);
  tri->children = 0;

  if (tri->parent) {
    if (tri->parent->children[0].children == 0 &&
        tri->parent->children[1].children == 0 &&
        tri->parent->children[2].children == 0 &&
        tri->parent->children[3].children == 0) {
      DL_APPEND(sphere->merge, tri->parent);
      tri->parent->type = 1;
    }
  }

  DL_DELETE(sphere->merge, tri);
  DL_APPEND(sphere->priority, tri);
}
