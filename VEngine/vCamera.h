#ifndef _VCAMERA_
#define _VCAMERA_
#include "vObject.h"

class vCamera : public vObject{
    protected:
        float fNear;
        float fFar;
        float fFov;
        float fAspectRatio;
        float fFovRad;

    public:
        int nScreenW, nScreenH;
        int nScreenOX, nScreenOY;
        float fOffsetX;
        float fOffsetY;
        mat4x4 matCamView; // Camera viewing matrix
        mat4x4 matCamProj; // Camera projection matrix

        vCamera(int nSw, int nSh, int nfx=0, int nfy=0, float fN=0.1f, float fF=1000.0f, float fV=90.0f)
                : nScreenW(nSw), nScreenH(nSh), nScreenOX(nfx), nScreenOY(nfy), fNear(fN), fFar(fF), fFov(fV){
            vObject();
            fAspectRatio = ((float)nScreenH/(float)nScreenW);
            fFovRad = 1.0f / tanf( (fV * 0.5f / 180.0f * 3.14159f));
            matCamProj = matMakeProjection(fFovRad, fAspectRatio, fNear, fFar);
            matCamView = matPointAt(vecLocation, vecForward, vecVertical);

            fOffsetX = (float)nfx/(float)nSw * 2;
            fOffsetY = (float)nfy/(float)nSh * 2;
        };

        void setMatProj(mat4x4& matProj){ matCamProj = matProj; };
        void ApplyTranslation(vec3d);
        void ApplyRotation(mat4x4& matRot, vec3d& vecPivot);
        void PointAt(vec3d& vecTarget);

    private:
        void update_camera_mat();

};

#endif//_VCAMERA_