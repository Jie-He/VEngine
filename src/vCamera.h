#ifndef _VCAMERA_
#define _VCAMERA_
#include "vObject.h"

class vCamera : public vObject{
    public:
        mat4x4 matCamView; // Camera viewing matrix
        mat4x4 matCamProj; // Camera projection matrix

        vCamera(){
            vObject();
            // Calculate viewing matrix from the three directional matrix
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