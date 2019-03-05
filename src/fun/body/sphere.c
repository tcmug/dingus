
#include "sphere.h"
#include "../../core/log.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #define PER_TRIANGLE (1)
#include "triangle.h"

#define TOP (0)
#define FRONTLEFT (1)
#define FRONTRIGHT (2)
#define BACKLEFT (3)
#define BACKRIGHT (4)
#define BOTTOM (5)

#define NORM1 (0.70710678118)

void bodysphere_create(bodysphere *sphere, real radius) {
  sphere->triangles = malloc(sizeof(bodytri) * 8);

  sphere->render_triangles = TW_Vector3BufferCreate(3 * 1000, GL_STREAM_DRAW);
  sphere->render_uvs = TW_Vector2BufferCreate(3 * 1000, GL_STREAM_DRAW);

  TW_Vector3 points[8] = {{0, 1, 0},          {-NORM1, 0, NORM1},
                          {NORM1, 0, NORM1},  {-NORM1, 0, -NORM1},
                          {NORM1, 0, -NORM1}, {0, -1, 0}};

  int i = 0;

  sphere->radius = radius;

  for (int j = 0; j < 8; j++) {
    points[j] = TW_Vector3MulReal(points[j], sphere->radius);
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

  sphere->position.x = 5;
  sphere->position.y = 0;
  sphere->position.z = 0;
  sphere->matrix = TW_MatrixIdentity();

  bodytri *temp, *tri;
  bodytri *split[1000];
  for (int i = 0; i < 3; i++) {
    int n = 0;
    DL_FOREACH(sphere->priority, tri) { split[n++] = tri; }
    for (int j = 0; j < n; j++) {
      bodytri_split(sphere, split[j]);
    }
    n = 0;
  }
}

#define SZ 1000

void bodysphere_update(bodysphere *sphere, TW_Vector3 viewer) {
  return;
  TW_Vector3 relative = TW_Vector3SubVector(viewer, sphere->position);

  // for (int i = 0; i < 3; i++) {
  //   relative[i] = viewer[i] - sphere->position[i];
  // }

  // vec4 relative4, temp = {relative[0], relative[1], relative[2], 1};
  // TW_Matrix inv;
  // mat4x4_invert(inv, sphere->matrix);
  // mat4x4_mul_vec4(relative4, inv, temp);
  // relative[0] = relative4[0];
  // relative[1] = relative4[1];
  // relative[2] = relative4[2];

#define SPLIT_TRESHOLD (0.1)
#define MERGE_TRESHOLD (0.1)
#define BACKFACE_TRESHOLD (-0.1)
#define MIN_TRIANGLE_SIZE (0.1)
#define MAX_TRIANGLE_SIZE (100)

  for (int i = 0; i < 1; i++) {

    bodytri *tri, *split[SZ], *merge[SZ];
    int n_split = 0, n_merge = 0;
    TW_Vector3 temp;

    DL_FOREACH(sphere->priority, tri) {
      real distance_to_viewer, tri_size;

      temp = TW_Vector3SubVector(relative, tri->points[0]);
      // oint3_sub(temp, relative, tri->points[0]);
      distance_to_viewer = TW_Vector3Length(temp);

      temp = TW_Vector3SubVector(tri->points[0], tri->points[1]);
      // point3_sub(temp, tri->points[0], tri->points[1]);
      tri_size = TW_Vector3Length(temp);

      TW_Vector3 viewn;
      viewn = TW_Vector3SubVector(relative, tri->points[0]);
      // point3_sub(viewn, relative, tri->points[0]);
      real d = TW_Vector3Dot(viewn, tri->normal);
      // point3_mul_inner(viewn, tri->normal);
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

      temp = TW_Vector3SubVector(relative, tri->points[0]);
      distance_to_viewer = TW_Vector3Length(temp);

      temp = TW_Vector3SubVector(tri->points[0], tri->points[1]);
      tri_size = TW_Vector3Length(temp);

      TW_Vector3 viewn;
      viewn = TW_Vector3SubVector(relative, tri->points[0]);
      real d = TW_Vector3Dot(viewn, tri->normal);

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

int at = 0;

void bodytri_render(bodysphere *sphere, bodytri *tri) {
  assert(tri->children == 0);

  if (at >= sphere->render_triangles->size) {
    TW_Vector3BufferUpdate(sphere->render_triangles, at);
    TW_Vector3BufferBind(sphere->render_triangles, 0);
    glDrawArrays(GL_TRIANGLES, 0, at);
    at = 0;
  }

  memcpy(&sphere->render_triangles->data[at], &tri->points,
         sizeof(TW_Vector3) * 3);

  sphere->render_uvs->data[at] = (TW_Vector2){0, 0};
  sphere->render_uvs->data[at + 1] = (TW_Vector2){1, 0};
  sphere->render_uvs->data[at + 2] = (TW_Vector2){0, 1};
  // sphere->render_triangles->data[at] = tri->points[0];
  // sphere->render_triangles->data[at + 1] = tri->points[1];
  // sphere->render_triangles->data[at + 2] = tri->points[2];

  TW_Vector3BufferUpdate(sphere->render_triangles, at);
  TW_Vector2BufferUpdate(sphere->render_uvs, at);

  at += 3;
}

#include "../../core/log.h"

void bodysphere_render(bodysphere *sphere) {
  at = 0;
  bodytri *tri;
  DL_FOREACH(sphere->priority, tri) bodytri_render(sphere, tri);

  if (at > 0) {
    TW_Vector3BufferBind(sphere->render_triangles, 0);
    TW_Vector2BufferBind(sphere->render_uvs, 1);

    glDrawArrays(GL_TRIANGLES, 0, at);
    at = 0;
  }
}