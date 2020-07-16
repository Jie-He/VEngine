#ifndef _VEC_MAT_
#define _VEC_MAT_

#include <vector>
#include <math.h>
#include <iostream>
#include <strstream>
#include <fstream>

// Quick 2D coord struct
struct vec2d{
    float x, y;

    vec2d(float x=0, float y=0) : x(x), y(y){}

    // Overload some operators
    vec2d& operator=(vec2d v){
		x = v.x;
		y = v.y;
		return *this;
	}

	vec2d operator+(const vec2d& v) const{
		return vec2d(v.x + x, v.y + y);
	}

	vec2d operator-(const vec2d& v) const{
		return vec2d(x - v.x, y - v.y);
	}

	vec2d operator==(const vec2d& v) const{
		return (x == v.x && y == v.y);
	}

	vec2d& operator+=(const vec2d& v){
		x += v.x;
		y += v.y;
		return *this;
	}
};

// 3D coord struct
struct vec3d{
    float x, y, z;
    float w; // well not part of the 3d vec really.

    vec3d(float x=0, float y=0, float z=0, float w=1):
          x(x), y(y), z(z), w(w) {}

    // Overload some operators
    vec3d& operator=(vec3d v){
		x = v.x;
		y = v.y;
        z = v.z;
        w = v.w;
		return *this;
	}

	vec3d operator+(const vec3d& v) const{
		return vec3d(v.x + x, v.y + y, v.z + z);
	}

	vec3d operator-(const vec3d& v) const{
		return vec3d(x - v.x, y - v.y, z - v.z);
	}

	bool operator==(const vec3d& v) const{
		return (x == v.x && y == v.y && z == v.z); // Maybe w?
	}

    bool operator!=(const vec3d& v) const{
        return !(x == v.x && y == v.y && z == v.z);
    }

    // scalar multiplication
    vec3d operator*(const float k) const{
        return vec3d(x * k, y * k, z * k);
    }

    // pairwise mulitplication
    vec3d operator*(const vec3d& v) const{
        return vec3d( x * v.x, y * v.y, z * v.z);
    }

    vec3d operator/(const float k) const{
        return vec3d(x / k, y / k, z / k);
    }

	vec3d& operator+=(const vec3d& v){
		x += v.x;
		y += v.y;
        z += v.z;
		return *this;
	}

    vec3d& operator-=(const vec3d& v){
		x -= v.x;
		y -= v.y;
        z -= v.z;
		return *this;
	}

    float dot(const vec3d& v){
        return x * v.x + y * v.y + z * v.z;
    }

    float length(){
        return sqrtf(dot(*this));
    }
};

struct mat4x4{
    float m[4][4] = {0};
};

// Operation on vector
vec3d vecCrossProduct(vec3d&, vec3d&);
vec3d vecNormalise(vec3d&);
vec3d vecNegative(vec3d&);
// Operation on vector && matrix
vec3d  matMultiplyVector(mat4x4&, vec3d&);
mat4x4 matIdentity();
mat4x4 matMakeTranslate(vec3d&);
mat4x4 matPointAt(vec3d &, vec3d&, vec3d&);
mat4x4 matQuickInverse(mat4x4&);
mat4x4 matMakeRotationX(float);
mat4x4 matMakeRotationY(float);
mat4x4 matMakeRotationZ(float);
// Location vector, Axis vector, degree
mat4x4 matMakeScale(float);
mat4x4 matMakeRotationAxis(vec3d&, float);
mat4x4 matMultiplyMatrix(mat4x4&, mat4x4&);
mat4x4 matMakeProjection(float fFovRad, float fAspectRatio, float fNear, float fFar);

struct triangle{
    vec3d p[3];
    vec3d colour;

    triangle(){
        colour = vec3d(255.0f, 255.0f, 255.0f);
    };

    triangle(vec3d va, vec3d vb, vec3d vc, int r=255, int g=255, int b=255){
        p[0] = va;
        p[1] = vb;
        p[2] = vc;
        colour.x = r;
        colour.y = g;
        colour.z = b;
    }
    // A base colour for this face?
};

#endif//_VEC_MAT_