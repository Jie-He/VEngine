#ifndef _VMESH_
#define _VMESH_

#include "vObject.h"

class vMesh : public vObject{
    protected:
        std::vector<triangle> tris;
        // Collection of materal colour
        bool hasColourList = false;
        // Index 0 is the base colour
        std::vector<vec3d> vecColour;
    public:
        bool setColour(int r, int g, int b);
        bool setColour(vec3d& vc);
        vMesh(int r=255, int g=255, int b=255){
            setColour(r,g,b);
        };
        bool LoadMaterialFile(std::string sFilename);
        bool LoadFromObjectFile(std::string sFilename);
        void ApplyRotation(mat4x4& matRot, vec3d& vecPivot);
        void ApplyScaling(mat4x4& matScale,vec3d& vecPivot);
        void ApplyTranslation(vec3d vecTrans);
        void setLocation(vec3d vecLoc);

        vec3d getBaseColour(){ return vecColour[0]; };
        std::vector<triangle>& getTris(){ return tris; };
    private:
        void ApplyMatrixOnTris(mat4x4& mat);
};

#endif//_VMESH_