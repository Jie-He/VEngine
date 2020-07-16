#ifndef _VMESH_
#define _VMESH_

#include "vObject.h"

class vMesh : public vObject{
    protected:
        std::vector<triangle> tris;
        // Base colour for a mesh
        vec3d baseColour;
    public:
        bool setColour(int r, int g, int b);
        bool setColour(vec3d& vc);
        vMesh(int r=255, int g=255, int b=255){
            setColour(r,g,b);
        };

        bool LoadFromObjectFile(std::string sFilename);
        void ApplyRotation(mat4x4& matRot, vec3d& vecPivot);
        void ApplyScaling(mat4x4& matScale,vec3d& vecPivot);
        void ApplyTranslation(vec3d& vecTrans);
        vec3d getBaseColour(){ return baseColour; };
        std::vector<triangle>& getTris(){ return tris; };
    private:
        void ApplyMatrixOnTris(mat4x4& mat);
};

#endif//_VMESH_