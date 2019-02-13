#ifndef DINGUS_FUNCTIONS_H
#define DINGUS_FUNCTIONS_H

#include "math.h"

#define print_vec(str, vec) printf("%s: %f %f %f\n", str, vec.x, vec.y, vec.z)

vector vector_add_vector(vector a, vector b);
vector vector_add_real(vector a, real b);
vector vector_sub_vector(vector a, vector b);
vector vector_sub_real(vector a, real b);
vector vector_mul_vector(vector a, vector b);
vector vector_mul_real(vector a, real b);
vector vector_div_vector(vector a, vector b);
vector vector_div_real(vector a, real b);
real vector_length_squared(vector a);
real vector_length(vector a);
vector vector_normalize(vector a);
real distance_between_vectors(vector a, vector b);
real squared_distance_between_vectors(vector a, vector b);
real vector_dot(vector a, vector b);
vector vector_cross(vector a, vector b);
vector vector_reflect(vector v, vector normal);

int rectangle_includes(rectangle r, point p);

matrix matrix_identity();
matrix matrix_transposed(matrix m);
vector matrix_mul_vector(matrix m, vector v);
matrix matrix_translation(matrix m, vector v);
matrix matrix_scaling(matrix m, vector v);
vector matrix_project_vector(matrix m, vector v);
matrix matrix_mul_matrix(matrix a, matrix b);
void matrix_gl_uniform(const char *name, matrix m);
matrix matrix_from_vector(vector eye, vector to, vector up);
matrix matrix_perspective_projection(real near, real far, real fov, real ratio);
matrix matrix_rotation(real psi, real theta, real phi);
matrix matrix_orthogonal_projection(real left, real right, real bottom,
                                    real top, real near, real far);

int plane_facing_direction(plane p, vector d);
real vector_distance_to_plane(vector a, plane p);

real sphere_distance_to_plane(sphere s, plane p);

plane triangle_to_plane(triangle tri);

real ray_intersect_plane(vector planeOrigin, vector planeNormal,
                         vector rayOrigin, vector rayVector);
real ray_intersect_sphere(vector rO, vector rV, vector sO, real sR);
vector sphere_sweep_plane(sphere s, vector v, plane p);
vector closest_point_on_line(vector a, vector b, vector p);
vector closest_point_on_triangle(vector a, vector b, vector c, vector p);
real sphere_sweep_triangle(sphere s, vector v, triangle t, vector *touch_point);
int test_triangles(collision *collision, int num_triangles,
                   triangle *triangles);
int point_in_triangle(vector point, triangle t);
real sphere_triangle_sweep(sphere s, vector v, triangle t);

#endif