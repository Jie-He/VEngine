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
        mat4x4 matCamView; // Camera viewing matrix
        mat4x4 matCamProj; // Camera projection matrix
        vCamera(int nSw, int nSh): nScreenW(nSw), nScreenH(nSh){
            vObject();
            // use the default values for mat projection
            fNear = 0.1f;
            fFar  = 1000.0f;
            fFov  = 90.0f;
            fAspectRatio  = ((float)nScreenW/(float)nScreenH);
            fFovRad       = 1.0f / tanf( (fFov * 0.5f / 180.0f * 3.14159f));
            matCamProj = matMakeProjection(fFovRad, fAspectRatio, fNear, fFar);
            // Calculate viewing matrix from the three directional matrix
            matCamView = matPointAt(vecLocation, vecForward, vecVertical);
        }

        vCamera(int nSw, int nSh, float fN, float fF, float fV)
                : nScreenW(nSw), nScreenH(nSh), fNear(fN), fFar(fF), fFov(fV){
            vObject();
            fAspectRatio = ((float)nScreenW/(float)nScreenH);
            fFovRad = 1.0f / tanf( (fV * 0.5f / 180.0f * 3.14159f));
            matCamProj = matMakeProjection(fFovRad, fAspectRatio, fNear, fFar);
            matCamView = matPointAt(vecLocation, vecForward, vecVertical);
        }

        vCamera(mat4x4 matProj){
            vObject();
            matCamProj = matProj;
            // Calculate viewing matrix from the three directional matrix
            matCamView = matPointAt(vecLocation, vecForward, vecVertical);
        }

        void setMatProj(mat4x4& matProj){ matCamProj = matProj; };
        void ApplyRotation(mat4x4& matRot, vec3d& vecPivot);
        void PointAt(vec3d& vecTarget);

};

#endif//_VCAMERA_