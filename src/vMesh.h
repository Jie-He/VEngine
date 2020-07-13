#ifndef _VMESH_
#define _VMESH_

#include "vObject.h"

class vMesh : public vObject{
    public:
        std::vector<triangle> tris;
        // Base colour for a mesh
        int nBaseR = 0;
        int nBaseG = 0;
        int nBaseB = 0;

        vMesh(int r=255, int g=255, int b=255)
            : nBaseR(r), nBaseG(g), nBaseB(b){};

        bool LoadFromObjectFile(std::string sFilename);
        void ApplyRotation(mat4x4& matRot, vec3d& vecPivot);
        void ApplyTranslation(vec3d& vecTrans);
};

#endif//_VMESH_