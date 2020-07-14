#ifndef _VMESH_
#define _VMESH_

#include "vObject.h"

class vMesh : public vObject{
    public:
        std::vector<triangle> tris;
        // Base colour for a mesh
        vec3d baseColour;

        bool setColour(int r, int g, int b);
        bool setColour(vec3d& vc);
        vMesh(int r=255, int g=255, int b=255){
            
        };

        bool LoadFromObjectFile(std::string sFilename);
        void ApplyRotation(mat4x4& matRot, vec3d& vecPivot);
        void ApplyTranslation(vec3d& vecTrans);
        vec3d getBaseColour(){ return baseColour; }
};

#endif//_VMESH_