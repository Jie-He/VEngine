#ifndef _VEC_MAT_
#define _VEC_MAT_

#include <vector>

// Quick 2D coord struct
struct vec2d{
    float x, y;
};

// 3D coord struct
struct vec3d{
    float x, y, z;
    float w;
};

struct triangle{
    vec3d p[3];
    // A base colour for this face?
};

struct mesh{
    std::vector<triangle> tris; 
};

struct mat4x4{
    float m[4][4];
};


// operation on vector && matrix
vec3d Matrix_MultiplyVector(mat4x4 &m, vec3d &i);

#endif//_VEC_MAT_