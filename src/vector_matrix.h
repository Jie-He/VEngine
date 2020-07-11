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

	vec3d operator==(const vec3d& v) const{
		return (x == v.x && y == v.y && z == v.z); // Maybe w?
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

// Operation on vector && matrix
vec3d  matMultiplyVector(mat4x4&, vec3d&);
mat4x4 matIdentity();
mat4x4 matMakeTranslate(vec3d&);
mat4x4 matMakeRotationX(float);
mat4x4 matMakeRotationY(float);
mat4x4 matMakeRotationZ(float);
mat4x4 matMultiplyMatrix(mat4x4&, mat4x4&);

struct triangle{
    vec3d p[3];
    // Default constructor
    //triangle(vec3d a = vec3d(), vec3d b = vec3d(), vec3d c = vec3d()){
    //    p[0] = a;
    //    p[1] = b;
    //    p[2] = c;
    //}
    // A base colour for this face?
};

struct mesh{
    std::vector<triangle> tris; 
    // Base colour for a mesh
    int nBaseR = 0;
    int nBaseG = 0;
    int nBaseB = 0;
    
    vec3d origin;

    // Maybe a sprite later
    bool LoadFromObjectFile(std::string sFilename){
        std::ifstream f(sFilename);
        if (!f.is_open())
            return false;

        // local cache of verts
        std::vector<vec3d> verts;

        while(!f.eof()){
            char line[128];
            f.getline(line, 128);

            std::strstream s;
            s << line;

            char junk;

            if(line[0] == 'v'){
                vec3d v;
                s >> junk >> v.x >> v.y >> v.z;
                verts.push_back(v);
            }

            if( line[0] == 'f'){
                int f[3];
                s >> junk >> f[0] >> f[1] >> f[2];
                //triangle trif = {verts[f[0]-1], verts[f[1]-1], verts[f[2]-1]};
                //trif.setColour(0, 255, 255);
                tris.push_back( {verts[f[0]-1], verts[f[1]-1], verts[f[2]-1]} );
            }
        }
        return true;
    }
    
    // Around a pivot
    void ApplyRotation(mat4x4& matRot, vec3d& vecPivot){
        vec3d temp(0.0f, 0.0f, 0.0f);
		temp = temp - vecPivot;
		// Testing
		mat4x4 matPivotP = matMakeTranslate( vecPivot );
		mat4x4 matPivotS = matMakeTranslate( temp   );

        // Weird. should be matP * matRot * matS
		// But matS * matRot * matP works, maybe its bcs hot mat * vec is done
		mat4x4 matRotPivot = matMultiplyMatrix(matRot,    matPivotP);
			   matRotPivot = matMultiplyMatrix(matPivotS, matRotPivot);

        for (size_t i = 0; i < tris.size(); i++){
			tris[i].p[0] = matMultiplyVector(matRotPivot, tris[i].p[0]);
			tris[i].p[1] = matMultiplyVector(matRotPivot, tris[i].p[1]);
			tris[i].p[2] = matMultiplyVector(matRotPivot, tris[i].p[2]);
	    }
    }

    void ApplyTranslation(vec3d& vecTrans){
        // Update the origin too 
        origin = origin + vecTrans;
        for (size_t i = 0; i < tris.size(); i++){
			tris[i].p[0] = tris[i].p[0] + vecTrans;
			tris[i].p[1] = tris[i].p[1] + vecTrans;
			tris[i].p[2] = tris[i].p[2] + vecTrans;
	    }
    }

};

#endif//_VEC_MAT_