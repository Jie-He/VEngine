#include "vMesh.h"

    bool vMesh::setColour(int r, int g, int b){
        if (r > 255 || r < 0) return false;
        if (g > 255 || g < 0) return false;
        if (b > 255 || b < 0) return false;
        baseColour.x = r;
        baseColour.y = g;
        baseColour.z = b;
        return true;
    }

    bool vMesh::setColour(vec3d& vc){
        return setColour(vc.x, vc.y, vc.z);
    }

    bool vMesh::LoadFromObjectFile(std::string sFilename){
        std::ifstream f(sFilename);
        if (!f.is_open())
            return false;

        // local cache of verts
        std::vector<vec3d> verts;
        // Clear the stuff in current triangle list
        tris.clear();
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
                tris.push_back( triangle(verts[f[0]-1], verts[f[1]-1], verts[f[2]-1], 255.0f) );
            }
        }
        return true;
    }
    
    // Around a pivot
    void vMesh::ApplyRotation(mat4x4& matRot, vec3d& vecPivot){
        vObject::ApplyRotation(matRot, vecPivot);
        // Update each vertex in the triangle
        for (size_t i = 0; i < tris.size(); i++){
			tris[i].p[0] = matMultiplyVector(matRotPivot, tris[i].p[0]);
			tris[i].p[1] = matMultiplyVector(matRotPivot, tris[i].p[1]);
			tris[i].p[2] = matMultiplyVector(matRotPivot, tris[i].p[2]);
	    }
    }

    void vMesh::ApplyTranslation(vec3d& vecTrans){
        vObject::ApplyTranslation(vecTrans);
        // Update the origin too 
        vecLocation += vecTrans;
        for (size_t i = 0; i < tris.size(); i++){
			tris[i].p[0] = tris[i].p[0] + vecTrans;
			tris[i].p[1] = tris[i].p[1] + vecTrans;
			tris[i].p[2] = tris[i].p[2] + vecTrans;
	    }
    }
