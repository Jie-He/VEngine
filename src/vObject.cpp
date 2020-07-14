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

void vObject::ApplyRotation(mat4x4& matRot, vec3d& vecPivot){
    vec3d vecToZero(0.0f, 0.0f, 0.0f);
    // Update all directional vector
    // the directions are always rotating about zero   
    vecForward    = matMultiplyVector(matRot, vecForward   );
    vecVertical   = matMultiplyVector(matRot, vecVertical  );
    vecHorizontal = matMultiplyVector(matRot, vecHorizontal);

    vecToZero = vecToZero - vecPivot;

    // Translation matrix to and from the zero vector
    mat4x4 matPivotP = matMakeTranslate( vecPivot  );
    mat4x4 matPivotS = matMakeTranslate( vecToZero );

    // Weird. should be matP * matRot * matS
	// But matS * matRot * matP works, maybe its bcs hot mat * vec is done
    matRotPivot = matMultiplyMatrix(matRot,      matPivotP);
    matRotPivot = matMultiplyMatrix(matPivotS, matRotPivot);

    // Incase its not rotation at object location
    vecLocation = matMultiplyVector(matRotPivot, vecLocation);
   
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
