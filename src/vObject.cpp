// ============================================================
//  Defining the functions from the vObject class
// ============================================================

#include "vObject.h"

void vObject::ApplyTranslation(vec3d& vecTrans){
    vecLocation  += vecTrans;  
    //vecForward   += vecTrans;
    //vecVertical  += vecTrans;
    //vecHorizontal+= vecTrans;
}

void vObject::ApplyRotation(mat4x4& matRot){
    // Update all directional vector
    // the directions are always rotating about zero  
    // Need to know where this thing is pivoting to.... sad
    vecForward = vecForward + vecLocation;
    vecVertical= vecVertical+ vecLocation;
    vecHorizontal= vecHorizontal + vecLocation; 
    vecForward    = matMultiplyVector(matRot, vecForward   );
    vecVertical   = matMultiplyVector(matRot, vecVertical  );
    vecHorizontal = matMultiplyVector(matRot, vecHorizontal);
    // Normalise it?
    vecForward = vecForward - vecLocation;
    vecHorizontal = vecHorizontal - vecLocation;
    vecVertical = vecVertical - vecLocation;


    // Incase its not rotation at object location
    vecLocation = matMultiplyVector(matRot, vecLocation);  
}

void vObject::ApplyScaling(mat4x4& matScale){
    vecLocation = matMultiplyVector(matScale, vecLocation);
}

void vObject::PointAt(vec3d& vecTarget){
    // Update new forward direction
    if (vecTarget != vecLocation){
        vecForward = vecTarget - vecLocation;
        vecForward = vecNormalise(vecForward);

        // New up direction
        vec3d temp = vecVertical;
        vecVertical = vecForward * (vecVertical.dot(vecForward));
        vecVertical = temp - vecVertical;
        vecVertical = vecNormalise(vecVertical);

        // New right direction
        vecHorizontal = vecCrossProduct(vecVertical, vecForward);
    }
}
