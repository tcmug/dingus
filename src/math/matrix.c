
#include "math.h"

#include "../core/engine.h"

matrix matrix_identity() {
  /*
  The identity matrix is a matrix with "zero" rotation, translation and scale:
    1 0 0 0
    0 1 0 0
    0 0 1 0
    0 0 0 1
  */
  return (matrix){1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
}

matrix matrix_transposed(matrix m) {
  /*
  Transposition of a matrix is simply swapping rows to columns, e.g.
  from:
    a b c d
    e f g h
    i j k l
    m n o p
  to:
    a e i m
    b f j n
    c g k o
    d h l p
  */
  return (matrix){m.a, m.e, m.i, m.m, m.b, m.f, m.j, m.n,
                  m.c, m.g, m.k, m.o, m.d, m.h, m.l, m.p};
}

vector matrix_mul_vector(matrix m, vector v) {
  /*
  Multiplication of a matrix with a vector (3) is:

    a b c d     x     ax + by + cz + dw
    e f g h  *  y  =  ex + fy + gz + hw
    i j k l     z     ix + fy + kz + lw
    m n o p     w     mx + ny + oz + pw

  */
  const real v_w = 1;
  return (vector){
      m.a * v.x + m.b + v.y + m.c * v.z + m.d * v_w,
      m.e * v.x + m.f + v.y + m.g * v.z + m.h * v_w,
      m.i * v.x + m.j + v.y + m.k * v.z + m.l * v_w
      // m.m * v.x + m.n + v.y + m.o * v.z + p.l * v.w
  };
}

matrix matrix_translation(matrix m, vector v) {
  /*
  Translation is:
    1 0 0 x
    0 1 0 y
    0 0 1 z
    0 0 0 1
  */
  return (matrix){1, 0, 0, v.x, 0, 1, 0, v.y, 0, 0, 1, v.z, 0, 0, 0, 1};
}

matrix matrix_scaling(matrix m, vector v) {
  /*
  Scaling by x,y,z is:
  from:
    x 0 0 0
    0 y 0 0
    0 0 z 0
    0 0 0 1
  */
  return (matrix){v.x, 0, 0, 0, 0, v.y, 0, 0, v.z, 0, 0, 0, 0, 1};
}

vector matrix_project_vector(matrix m, vector v) {
  vector out;
  out.x = v.x * m.value[0] + v.y * m.value[4] + v.z * m.value[8] + m.value[12];
  out.y = v.x * m.value[1] + v.y * m.value[5] + v.z * m.value[9] + m.value[13];
  out.z = v.x * m.value[2] + v.y * m.value[6] + v.z * m.value[10] + m.value[14];
  real t = 1.0 / (v.x * m.value[3] + v.y * m.value[7] + v.z * m.value[11] +
                  m.value[15]);
  return vector_mul_real(out, t);
}

matrix matrix_mul_matrix(matrix a, matrix b) {
  return (matrix){a.value[0] * b.value[0] + a.value[4] * b.value[1] +
                      a.value[8] * b.value[2] + a.value[12] * b.value[3],
                  a.value[1] * b.value[0] + a.value[5] * b.value[1] +
                      a.value[9] * b.value[2] + a.value[13] * b.value[3],
                  a.value[2] * b.value[0] + a.value[6] * b.value[1] +
                      a.value[10] * b.value[2] + a.value[14] * b.value[3],
                  a.value[3] * b.value[0] + a.value[7] * b.value[1] +
                      a.value[11] * b.value[2] + a.value[15] * b.value[3],
                  a.value[0] * b.value[4] + a.value[4] * b.value[5] +
                      a.value[8] * b.value[6] + a.value[12] * b.value[7],
                  a.value[1] * b.value[4] + a.value[5] * b.value[5] +
                      a.value[9] * b.value[6] + a.value[13] * b.value[7],
                  a.value[2] * b.value[4] + a.value[6] * b.value[5] +
                      a.value[10] * b.value[6] + a.value[14] * b.value[7],
                  a.value[3] * b.value[4] + a.value[7] * b.value[5] +
                      a.value[11] * b.value[6] + a.value[15] * b.value[7],
                  a.value[0] * b.value[8] + a.value[4] * b.value[9] +
                      a.value[8] * b.value[10] + a.value[12] * b.value[11],
                  a.value[1] * b.value[8] + a.value[5] * b.value[9] +
                      a.value[9] * b.value[10] + a.value[13] * b.value[11],
                  a.value[2] * b.value[8] + a.value[6] * b.value[9] +
                      a.value[10] * b.value[10] + a.value[14] * b.value[11],
                  a.value[3] * b.value[8] + a.value[7] * b.value[9] +
                      a.value[11] * b.value[10] + a.value[15] * b.value[11],
                  a.value[0] * b.value[12] + a.value[4] * b.value[13] +
                      a.value[8] * b.value[14] + a.value[12] * b.value[15],
                  a.value[1] * b.value[12] + a.value[5] * b.value[13] +
                      a.value[9] * b.value[14] + a.value[13] * b.value[15],
                  a.value[2] * b.value[12] + a.value[6] * b.value[13] +
                      a.value[10] * b.value[14] + a.value[14] * b.value[15],
                  a.value[3] * b.value[12] + a.value[7] * b.value[13] +
                      a.value[11] * b.value[14] + a.value[15] * b.value[15]};
}

void matrix_gl_uniform(const char *name, matrix m) {
  GLint program;
  glGetIntegerv(GL_CURRENT_PROGRAM, &program);
  // engine_gl_check();
  GLint loc = glGetUniformLocation(program, name);
  // engine_gl_check();
  glUniformMatrix4fv(loc, 1, 0, m.value);
  // engine_gl_check();
}

matrix matrix_from_vector(vector eye, vector to, vector up) {

  vector zaxis = vector_normalize(vector_sub_vector(eye, to));
  vector xaxis = vector_normalize(vector_cross(up, zaxis));
  vector yaxis = vector_cross(zaxis, xaxis);

  return (matrix){xaxis.x,
                  yaxis.x,
                  zaxis.x,
                  0,
                  xaxis.y,
                  yaxis.y,
                  zaxis.y,
                  0,
                  xaxis.z,
                  yaxis.z,
                  zaxis.z,
                  0,
                  -vector_dot(xaxis, eye),
                  -vector_dot(yaxis, eye),
                  -vector_dot(zaxis, eye),
                  1};
}

matrix matrix_perspective_projection(real near, real far, real fov,
                                     real ratio) {

  matrix m = matrix_identity();

  real r = TO_RAD(fov) / 2;
  real a = ratio;
  real f = 1 / tan(r); // xzoom
  real dp = near - far;

  /*
      a b c d
      e f g h
      i j k l
      m n o p
  */

  m.value[0] = f / a;
  m.value[5] = f;
  m.value[10] = (far + near) / dp;
  m.value[14] = (2 * far * near) / dp; // 32
  m.value[11] = -1;                    // 23
  m.value[15] = 0;                     // TÄRKEÄ

  return m;
}

matrix matrix_rotation(real psi, real theta, real phi) {

  real ca = cos(phi), sa = sin(phi);
  real cb = cos(theta), sb = sin(theta);
  real cy = cos(psi), sy = sin(psi);
  return (matrix){ca * cb,
                  sa * cb,
                  -sb,
                  0.0,
                  ca * sb * sy - sa * cy,
                  sa * sb * sy + ca * cy,
                  cb * sy,
                  0.0,
                  ca * sb * cy + sa * sy,
                  sa * sb * cy - ca * sy,
                  cb * cy,
                  0.0,
                  0.0,
                  0.0,
                  0.0,
                  1.0};
}

matrix matrix_orthogonal_projection(real left, real right, real bottom,
                                    real top, real near, real far) {

  return (matrix){2 / (right - left),
                  0,
                  0,
                  0,
                  0,
                  2 / (top - bottom),
                  0,
                  0,
                  0,
                  0,
                  -2 / (far - near),
                  0,
                  -(right + left) / (right - left),
                  -(top + bottom) / (top - bottom),
                  -(far + near) / (far - near),
                  1};
}

/*
                m4 &reverse_projection_perspective(real near, real far, real
   fov, real ratio) {

                        this->identity();

                        real r = TO_RAD(fov) / 2;
                        real a = ratio;
                        real f = 1 / tan(r); // xzoom
                        real dp = near - far;

                        this->values[0] = a / f;
                        this->values[5] = 1 / f;
                        this->values[10] = 0;
                        this->values[14] = -1;
                        this->values[11] = dp / (2 * far * near);
                        this->values[15] = (far + near) / (2 * far * near);

                        return *this;
                }
*/

// https://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/
