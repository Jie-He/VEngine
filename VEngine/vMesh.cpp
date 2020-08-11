#include "vMesh.h"
#include <cstring>

    bool vMesh::setColour(int r, int g, int b){
        if (r > 255 || r < 0) return false;
        if (g > 255 || g < 0) return false;
        if (b > 255 || b < 0) return false;
        vec3d baseColour;
        baseColour.x = r;
        baseColour.y = g;
        baseColour.z = b;
        if (vecColour.size() == 0)
            vecColour.push_back(baseColour);
        else
            vecColour[0] = baseColour;       

        // Update all tri's colour
        if (!hasColourList){
            for (auto &t : tris){
                t.colour = baseColour;
            }
        }
        return true;
    }

    bool vMesh::setColour(vec3d& vc){
        return setColour(vc.x, vc.y, vc.z);
    }

    bool vMesh::LoadMaterialFile(std::string sFilename){
        std::ifstream f(sFilename);
        if (!f.is_open())
            return false;
        while(!f.eof()){
            char line[128];
            f.getline(line, 128);
            std::strstream s;
            s << line;

            vec3d temp;
            char kd[3] = "Kd";
            char word[3];
            s >> word;
            
            if( strcmp(word, kd) == 0){
                // read the colour information
                s >> temp.x >> temp.y >> temp.z;
               
                // scale to 255
                temp.x *= 255;
                temp.y *= 255;
                temp.z *= 255;
                vecColour.push_back(temp);
            }
        }
        hasColourList = true;
        return true;
    }

    bool vMesh::LoadFromObjectFile(std::string sFilename){
        std::ifstream f(sFilename);
        if (!f.is_open())
            return false;

        // local cache of verts
        std::vector<vec3d> verts;

        short sColourIndex = 0;
        char usemtl[7] = "usemtl";
        char mtllib[7] = "mtllib";


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
            }else if( line[0] == 'f'){
                int f[3];
                s >> junk >> f[0] >> f[1] >> f[2];
                tris.push_back( triangle(verts[f[0]-1], verts[f[1]-1], verts[f[2]-1], vecColour[sColourIndex]));
            }else{
                // Try if this is a colour file or use material 
                char word[7] = "nonono";
                s >> word;

                if (strcmp(mtllib, word) == 0){
                    // read the file
                    std::string mtlfile;
                    s >> mtlfile;

                    std::cout << "Found mtlfile" << std::endl;

                    // For linux & vita file sys, windows uses '\'
                    const size_t last_slash_idx = sFilename.rfind('/');
                    if (std::string::npos != last_slash_idx){
                        std::string directory;
                        directory = sFilename.substr(0, last_slash_idx + 1);
                        directory.append(mtlfile);
                        std::cout << "Mtl Filepath: " << directory << std::endl;
                        LoadMaterialFile(directory);
                    }
                }


                if (hasColourList){
                    if( strcmp(usemtl, word) == 0){
                        sColourIndex ++;
                    }
                }
            }            
        }
        return true;
    }
    
    // Around a pivot
    void vMesh::ApplyRotation(mat4x4& matRot, vec3d& vecPivot){
        vObject::ApplyRotation(matRot, vecPivot);
        // Update each vertex in the triangle
        ApplyMatrixOnTris(matPivot);
    }

    void vMesh::ApplyScaling(mat4x4& matScale, vec3d& vecPivot){
        vObject::ApplyScaling(matScale, vecPivot);
        ApplyMatrixOnTris(matPivot);
    }

    void vMesh::ApplyTranslation(vec3d vecTrans){
        // Update the origin
        vObject::ApplyTranslation(vecTrans);
        // Update the mesh vertex
        for (size_t i = 0; i < tris.size(); i++){
			tris[i].p[0] = tris[i].p[0] + vecTrans;
			tris[i].p[1] = tris[i].p[1] + vecTrans;
			tris[i].p[2] = tris[i].p[2] + vecTrans;
	    }
    }

    void vMesh::setLocation(vec3d vecLoc){
        if (vecLoc != vecLocation){
            // Calc translation vector to new location
            vec3d vecTranslate = vecLoc - vecLocation;
            ApplyTranslation(vecTranslate);
        }
    }

    void vMesh::ApplyMatrixOnTris(mat4x4& mat){
        for (size_t i = 0; i < tris.size(); i++){
			tris[i].p[0] = matMultiplyVector(mat, tris[i].p[0]);
			tris[i].p[1] = matMultiplyVector(mat, tris[i].p[1]);
			tris[i].p[2] = matMultiplyVector(mat, tris[i].p[2]);
	    }
    }
