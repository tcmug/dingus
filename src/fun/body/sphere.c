
#include "sphere.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #define PER_TRIANGLE (1)

#define TOP (0)
#define FRONTLEFT (1)
#define FRONTRIGHT (2)
#define BACKLEFT (3)
#define BACKRIGHT (4)
#define BOTTOM (5)

#define NORM1 (0.70710678118)

void bodysphere_create(bodysphere *sphere, real radius) {
  sphere->triangles = malloc(sizeof(bodytri) * 8);

  TW_Vector3 points[8] = {{0, 1, 0},          {-NORM1, 0, NORM1},
                          {NORM1, 0, NORM1},  {-NORM1, 0, -NORM1},
                          {NORM1, 0, -NORM1}, {0, -1, 0}};

  int i = 0;

  sphere->radius = radius;

  for (int j = 0; j < 8; j++) {
    point3_scale(points[j], points[j], sphere->radius);
  }

  bodytri_create(&sphere->triangles[i++],
                 POINTS(points[TOP], points[FRONTRIGHT], points[FRONTLEFT]));
  bodytri_create(&sphere->triangles[i++],
                 POINTS(points[TOP], points[BACKRIGHT], points[FRONTRIGHT]));
  bodytri_create(&sphere->triangles[i++],
                 POINTS(points[TOP], points[BACKLEFT], points[BACKRIGHT]));
  bodytri_create(&sphere->triangles[i++],
                 POINTS(points[TOP], points[FRONTLEFT], points[BACKLEFT]));
  bodytri_create(&sphere->triangles[i++],
                 POINTS(points[FRONTLEFT], points[FRONTRIGHT], points[BOTTOM]));
  bodytri_create(&sphere->triangles[i++],
                 POINTS(points[FRONTRIGHT], points[BACKRIGHT], points[BOTTOM]));
  bodytri_create(&sphere->triangles[i++],
                 POINTS(points[BACKRIGHT], points[BACKLEFT], points[BOTTOM]));
  bodytri_create(&sphere->triangles[i++],
                 POINTS(points[BACKLEFT], points[FRONTLEFT], points[BOTTOM]));

  // Append triangles to priority list.
  sphere->priority = 0;
  sphere->merge = 0;
  for (i = 0; i < 8; i++) {
    DL_APPEND(sphere->priority, &sphere->triangles[i]);
  }

  sphere->position[0] = 5;
  sphere->position[1] = 0;
  sphere->position[2] = 0;
  mat4x4_identity(sphere->matrix);
}

#define SZ 1000

void bodysphere_update(bodysphere *sphere, TW_Vector3 viewer) {

  TW_Vector3 relative;
  for (int i = 0; i < 3; i++) {
    relative[i] = viewer[i] - sphere->position[i];
  }

  vec4 relative4, temp = {relative[0], relative[1], relative[2], 1};
  TW_Matrix inv;
  mat4x4_invert(inv, sphere->matrix);
  mat4x4_mul_vec4(relative4, inv, temp);
  relative[0] = relative4[0];
  relative[1] = relative4[1];
  relative[2] = relative4[2];

#define SPLIT_TRESHOLD (0.1)
#define MERGE_TRESHOLD (0.1)
#define BACKFACE_TRESHOLD (-0.1)
#define MIN_TRIANGLE_SIZE (1)
#define MAX_TRIANGLE_SIZE (500)

  for (int i = 0; i < 1; i++) {

    bodytri *tri, *split[SZ], *merge[SZ];
    int n_split = 0, n_merge = 0;
    TW_Vector3 temp;

    DL_FOREACH(sphere->priority, tri) {
      real distance_to_viewer, tri_size;

      point3_sub(temp, relative, tri->points[0]);
      distance_to_viewer = point3_len(temp);

      point3_sub(temp, tri->points[0], tri->points[1]);
      tri_size = point3_len(temp);

      TW_Vector3 viewn;
      point3_sub(viewn, relative, tri->points[0]);
      real d = point3_mul_inner(viewn, tri->normal);
      if ((d >= BACKFACE_TRESHOLD || tri_size > MAX_TRIANGLE_SIZE) &&
          tri_size > MIN_TRIANGLE_SIZE &&
          tri_size / distance_to_viewer > SPLIT_TRESHOLD) {
        split[n_split++] = tri;
      }
      if (n_split == SZ) {
        break;
      }
    }

    for (int i = 0; i < n_split; i++) {
      bodytri_split(sphere, split[i]);
    }

    DL_FOREACH(sphere->merge, tri) {
      real distance_to_viewer, tri_size;
      point3_sub(temp, relative, tri->points[0]);
      distance_to_viewer = point3_len(temp);

      point3_sub(temp, tri->points[0], tri->points[1]);
      tri_size = point3_len(temp);

      TW_Vector3 viewn;
      point3_sub(viewn, relative, tri->points[0]);
      real d = point3_mul_inner(viewn, tri->normal);

      if ((d < BACKFACE_TRESHOLD || tri_size < MAX_TRIANGLE_SIZE) &&
          tri_size / distance_to_viewer < MERGE_TRESHOLD) {
        merge[n_merge++] = tri;
      }
      if (n_merge == SZ) {
        break;
      }
    }

    for (int i = 0; i < n_merge; i++) {
      bodytri_merge(sphere, merge[i]);
    }
    if (n_split > 0 || n_merge > 0)
      printf("splits: %u, merges: %u\n", n_split, n_merge);
  }
}