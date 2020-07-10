#ifndef _VEC_MAT_
#define _VEC_MAT_

#include <vector>
#include <math.h>

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

	vec3d operator==(const vec3d& v) const{
		return (x == v.x && y == v.y && z == v.z); // Maybe w?
	}

    // scalar multiplication
    vec3d operator*(const float k) const{
        return vec3d(x * k, y * k, z * k, w);
    }

    vec3d operator/(const float k) const{
        return vec3d(x / k, y / k, z / k, w);
    }

    // pairwise mulitplication
    vec3d operator*(const vec3d& v) const{
        return vec3d( x * v.x, y * v.y, z * v.z);
    }

	vec3d& operator+=(const vec3d& v){
		x += v.x;
		y += v.y;
        z += v.z;
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

struct triangle{
    vec3d p[3];
    // A base colour for this face?
};

struct mesh{
    std::vector<triangle> tris; 
    // Base colour for a mesh
    int nBaseR = 0;
    int nBaseG = 0;
    int nBaseB = 0;

    // Maybe a sprite later
};

// Operation on vector
vec3d vecCrossProduct(vec3d&, vec3d&);
vec3d vecNormalise(vec3d&);

// Operation on vector && matrix
vec3d matMultiplyVector(mat4x4&, vec3d&);
mat4x4 matIdentity();
mat4x4 matMakeRotationX(float);
mat4x4 matMakeRotationY(float);
mat4x4 matMakeRotationZ(float);

#endif//_VEC_MAT_