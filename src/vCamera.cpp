// ============================================================
//  Defining the functions from vCamera class
// ============================================================
#include "vCamera.h"


void vCamera::ApplyRotation(mat4x4& matRot){
    // Needs to recalculate if vecFroward since vecForward is altered
    vObject::ApplyRotation(matRot);
    vec3d vecLook = vecLocation + vecForward;
    vCamera::PointAt(vecLook);
}

// Calls the parent function first for adjusting 
void vCamera::PointAt(vec3d& vecTarget){
    vObject::PointAt(vecTarget);

    matCamView = matMakeTranslate(vecLocation);
    matCamView.m[0][0] = vecHorizontal.x;	matCamView.m[0][1] = vecHorizontal.y;	matCamView.m[0][2] = vecHorizontal.z;	
    matCamView.m[1][0] = vecVertical.x;		matCamView.m[1][1] = vecVertical.y;		matCamView.m[1][2] = vecVertical.z;		
    matCamView.m[2][0] = vecForward.x;	    matCamView.m[2][1] = vecForward.y;	    matCamView.m[2][2] = vecForward.z;

    matCamView = matQuickInverse(matCamView);
}