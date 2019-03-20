
#include "object.h"
#include "utlist.h"

TW_Model *models = 0;

TW_Model *TW_ModelLoad(const char *name) {

  TW_Model *model = (TW_Model *)TW_ResourceFind((TW_Resource *)models, name);
  if (model) {
    app_log("Model loaded from cache (%s)", name);
    return model;
  }

  model = (TW_Model *)malloc(sizeof(TW_Model));

  FILE *file = fopen(name, "r");

  if (!file) {
    printf("No file %s\n", name);
    return 0;
  }

  char line[256];

#define MAX_DATA_POINTS (20000)

  int num_vertices = 0;
  TW_Vector3 vertices[MAX_DATA_POINTS];

  int num_normals = 0;
  TW_Vector3 normals[MAX_DATA_POINTS];

  int num_uvs = 0;
  TW_Vector2 uvs[MAX_DATA_POINTS];

  int num_indices = 0;
  int indices[MAX_DATA_POINTS];
  int normal_indices[MAX_DATA_POINTS];
  int texture_indices[MAX_DATA_POINTS];

  printf("Loading...\n");

  while (fgets(line, sizeof(line), file)) {
    float x, y, z;
    int avi, bvi, cvi, ani, bni, cni, ati, bti, cti, dvi, dti, dni;
    if (sscanf(line, "v %f %f %f", &x, &y, &z) == 3) {
      // printf("VERTICES: %f %f %f\n", x, y, z);
      vertices[num_vertices++] = (TW_Vector3){x, y, z};

    } else if (sscanf(line, "vn %f %f %f", &x, &y, &z) == 3) {
      // printf("NORMAL: %f %f %f\n", x, y, z);
      normals[num_normals++] = (TW_Vector3){x, y, z};

    } else if (sscanf(line, "vt %f %f", &x, &y) == 2) {
      // printf("TEXTURE: %f %f\n", x, y);
      uvs[num_uvs++] = (TW_Vector2){x, y};

    } else if (sscanf(line, "f %i//%i %i//%i %i//%i %i//%i", &avi, &ani, &bvi,
                      &bni, &cvi, &cni, &dvi, &dni) == 8) {
      // printf("FACE: %i %i %i\n", avi, bvi, cvi);
      normal_indices[num_indices] = ani - 1;
      indices[num_indices++] = avi - 1;

      normal_indices[num_indices] = cni - 1;
      indices[num_indices++] = cvi - 1;

      normal_indices[num_indices] = bni - 1;
      indices[num_indices++] = bvi - 1;

      // printf("FACE: %i %i %i\n", avi, bvi, cvi);
      normal_indices[num_indices] = ani - 1;
      indices[num_indices++] = avi - 1;

      normal_indices[num_indices] = dni - 1;
      indices[num_indices++] = dvi - 1;

      normal_indices[num_indices] = cni - 1;
      indices[num_indices++] = cvi - 1;

    }

    else if (sscanf(line, "f %i/%i/%i %i/%i/%i %i/%i/%i", &avi, &ati, &ani,
                    &bvi, &bti, &bni, &cvi, &cti, &cni) == 9) {
      // printf("FACE: %i %i %i\n", avi, bvi, cvi);
      normal_indices[num_indices] = ani - 1;
      texture_indices[num_indices] = ati - 1;
      indices[num_indices++] = avi - 1;

      normal_indices[num_indices] = bni - 1;
      texture_indices[num_indices] = bti - 1;
      indices[num_indices++] = cvi - 1;

      normal_indices[num_indices] = cni - 1;
      texture_indices[num_indices] = cti - 1;
      indices[num_indices++] = bvi - 1;

    } else if (sscanf(line, "f %i//%i %i//%i %i//%i", &avi, &ani, &bvi, &bni,
                      &cvi, &cni) == 6) {
      // printf("FACE: %i %i %i\n", avi, bvi, cvi);
      normal_indices[num_indices] = ani - 1;
      indices[num_indices++] = avi - 1;

      normal_indices[num_indices] = bni - 1;
      indices[num_indices++] = cvi - 1;

      normal_indices[num_indices] = cni - 1;
      indices[num_indices++] = bvi - 1;
    }
  }

  fclose(file);

  printf("%u vertices, %u normals, %u indices\n", num_vertices, num_normals,
         num_indices);

  TW_Vector3BufferInit(&model->vertices, num_vertices, GL_STATIC_DRAW);
  TW_Vector3BufferInit(&model->normals, num_vertices, GL_STATIC_DRAW);
  TW_Vector2BufferInit(&model->uvs, num_vertices, GL_STATIC_DRAW);
  TW_IndicesBufferInit(&model->indices, num_indices, GL_STATIC_DRAW);

  // memcpy(&model->vertices.data[0], &vertices, sizeof(TW_Vector3) *
  // num_indices);

  for (int i = 0; i < num_indices; i++) {
    model->indices.data[i] = indices[i];
    model->normals.data[indices[i]] = normals[normal_indices[i]];
  }

  for (int i = 0; i < num_vertices; i++) {
    model->vertices.data[i] = vertices[i];
    model->uvs.data[i] = (TW_Vector2){0, 0};
  }

  TW_Vector3BufferUpdate(&model->vertices, -1);
  TW_Vector3BufferUpdate(&model->normals, -1);
  TW_Vector2BufferUpdate(&model->uvs, -1);
  TW_IndicesBufferUpdate(&model->indices, -1);

  models = (TW_Model *)TW_ResourceAdd((TW_Resource *)models, name,
                                      (TW_Resource *)model);

  return model;
}

void TW_ModelCalculateNormals(TW_Model *self) {

  for (int i = 0; i < self->indices.size; i += 3) {
    const int a = self->indices.data[i];
    const int b = self->indices.data[i + 1];
    const int c = self->indices.data[i + 2];

    const TW_Vector3 va = self->vertices.data[a];
    const TW_Vector3 vb = self->vertices.data[b];
    const TW_Vector3 vc = self->vertices.data[c];

    TW_Vector3 normal =
        TW_Vector3Cross(TW_Vector3Normalize(TW_Vector3SubVector(vc, va)),
                        TW_Vector3Normalize(TW_Vector3SubVector(vb, va)));

    self->normals.data[a] = TW_Vector3AddVector(self->normals.data[a], normal);
    self->normals.data[b] = TW_Vector3AddVector(self->normals.data[b], normal);
    self->normals.data[c] = TW_Vector3AddVector(self->normals.data[c], normal);
  }

  for (int i = 0; i < self->vertices.size; i++) {
    self->normals.data[i] = TW_Vector3Normalize(self->normals.data[i]);
  }

  TW_Vector3BufferUpdate(&self->normals, -1);
}

TW_Model *TW_ModelClone(const TW_Model *self) {
  TW_Model *model = (TW_Model *)malloc(sizeof(TW_Model));

  TW_Vector3BufferInit(&model->vertices, self->vertices.size, GL_STATIC_DRAW);
  TW_Vector3BufferInit(&model->normals, self->vertices.size, GL_STATIC_DRAW);
  TW_Vector2BufferInit(&model->uvs, self->vertices.size, GL_STATIC_DRAW);
  TW_IndicesBufferInit(&model->indices, self->indices.size, GL_STATIC_DRAW);

  memcpy(&model->vertices.data[0], &self->vertices.data[0],
         sizeof(TW_Vector3) * self->vertices.size);
  memcpy(&model->normals.data[0], &self->normals.data[0],
         sizeof(TW_Vector3) * self->normals.size);
  memcpy(&model->uvs.data[0], &self->uvs.data[0],
         sizeof(TW_Vector2) * self->uvs.size);
  memcpy(&model->indices.data[0], &self->indices.data[0],
         sizeof(GLuint) * self->indices.size);

  TW_Vector3BufferUpdate(&model->vertices, -1);
  TW_Vector3BufferUpdate(&model->normals, -1);
  TW_Vector2BufferUpdate(&model->uvs, -1);
  TW_IndicesBufferUpdate(&model->indices, -1);

  return model;
}

#define VERTEX_MERGE_TRESHOLD (0.01)

TW_Model *TW_ModelSmoothen(const TW_Model *self) {

  TW_Model *model = TW_ModelClone(self);

  model->vertices.size = 0;

  for (int i = 0; i < self->indices.size; i++) {
    int match = -1;
    const TW_Vector3 a = self->vertices.data[self->indices.data[i]];
    // Look for a in existing vertices within limits.
    for (int j = 0; j < model->vertices.size; j++) {
      const TW_Vector3 b = model->vertices.data[j];
      if (fabs(a.x - b.x) < VERTEX_MERGE_TRESHOLD &&
          fabs(a.y - b.y) < VERTEX_MERGE_TRESHOLD &&
          fabs(a.z - b.z) < VERTEX_MERGE_TRESHOLD) {
        match = j;
        break;
      }
    }
    // Check: If it was not found in list, add to it:
    if (match == -1) {
      match = model->vertices.size;
      model->vertices.data[model->vertices.size++] = a;
    }
    model->indices.data[i] = match;
  }

  model->vertices.data =
      realloc(model->vertices.data, model->vertices.size * sizeof(TW_Vector3)),

  TW_Vector3BufferUpdate(&model->vertices, -1);
  TW_Vector2BufferUpdate(&model->uvs, -1);
  TW_IndicesBufferUpdate(&model->indices, -1);

  TW_ModelCalculateNormals(model);

  return model;
}