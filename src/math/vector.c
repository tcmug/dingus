
#include "math.h"

#include "../core/engine.h"

TW_Vector3 TW_Vector3AddVector(TW_Vector3 a, TW_Vector3 b) {
  return (TW_Vector3){a.x + b.x, a.y + b.y, a.z + b.z};
}

TW_Vector3 TW_Vector3AddReal(TW_Vector3 a, real b) {
  return (TW_Vector3){a.x + b, a.y + b, a.z + b};
}

TW_Vector3 TW_Vector3SubVector(TW_Vector3 a, TW_Vector3 b) {
  return (TW_Vector3){a.x - b.x, a.y - b.y, a.z - b.z};
}

TW_Vector3 TW_Vector3SubReal(TW_Vector3 a, real b) {
  return (TW_Vector3){a.x - b, a.y - b, a.z - b};
}

TW_Vector3 TW_Vector3MulVector(TW_Vector3 a, TW_Vector3 b) {
  return (TW_Vector3){a.x * b.x, a.y * b.y, a.z * b.z};
}

TW_Vector3 TW_Vector3MulReal(TW_Vector3 a, real b) {
  return (TW_Vector3){a.x * b, a.y * b, a.z * b};
}

TW_Vector3 TW_Vector3DivVector(TW_Vector3 a, TW_Vector3 b) {
  return (TW_Vector3){a.x / b.x, a.y / b.y, a.z / b.z};
}

TW_Vector3 TW_Vector3DivReal(TW_Vector3 a, real b) {
  return (TW_Vector3){a.x / b, a.y / b, a.z / b};
}

real TW_Vector3LengthSquared(TW_Vector3 a) {
  return (a.x * a.x) + (a.y * a.y) + (a.z * a.z);
}

real TW_Vector3Length(TW_Vector3 a) {
  return sqrtf(TW_Vector3LengthSquared(a));
}

TW_Vector3 TW_Vector3Normalize(TW_Vector3 a) {
  return TW_Vector3MulReal(a, 1.0 / TW_Vector3Length(a));
}

real TW_Vector3sDistance(TW_Vector3 a, TW_Vector3 b) {
  return TW_Vector3Length(TW_Vector3SubVector(b, a));
}

real TW_Vector3sSquaredDistance(TW_Vector3 a, TW_Vector3 b) {
  return TW_Vector3LengthSquared(TW_Vector3SubVector(b, a));
}

TW_Vector3 TW_Vector3Midpoint(TW_Vector3 a, TW_Vector3 b) {
  return TW_Vector3MulReal(TW_Vector3AddVector(a, b), 0.5);
}

real TW_Vector3Dot(TW_Vector3 a, TW_Vector3 b) {
  return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

TW_Vector3 TW_Vector3Cross(TW_Vector3 a, TW_Vector3 b) {
  return (TW_Vector3){(a.y * b.z) - (a.z * b.y), (a.z * b.x) - (a.x * b.z),
                      (a.x * b.y) - (a.y * b.x)};
}

TW_Vector3 TW_Vector3Reflect(TW_Vector3 v, TW_Vector3 normal) {
  return TW_Vector3AddVector(
      TW_Vector3MulReal(normal, -2 * TW_Vector3Dot(v, normal)), v);
}

void TW_Vector3GLUniform(const char *name, TW_Vector3 c) {
  GLint program;
  glGetIntegerv(GL_CURRENT_PROGRAM, &program);
  GLint loc = glGetUniformLocation(program, name);
  glUniform3fv(loc, 1, c.value);
}

void TW_Vector3GLUniformLoc(int loc, TW_Vector3 c) {
  glUniform3fv(loc, 1, c.value);
}