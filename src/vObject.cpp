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
    // Update all directional vector
    // the directions are always rotating about zero  
    vecForward    = matMultiplyVector(matRot, vecForward   );
    vecVertical   = matMultiplyVector(matRot, vecVertical  );
    vecHorizontal = matMultiplyVector(matRot, vecHorizontal);

    matPivot = makeRotationPivot(matRot, vecPivot);

    // Incase its not rotation at object location
    vecLocation = matMultiplyVector(matPivot, vecLocation);  
}

void vObject::ApplyScaling(mat4x4& matScale, vec3d& vecPivot){
    matPivot = makeRotationPivot(matScale, vecPivot);
    vecLocation = matMultiplyVector(matPivot, vecLocation);
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

mat4x4 vObject::makeRotationPivot(mat4x4& matRot, vec3d& vecPivot){
    mat4x4 matrix;
    vec3d vecToZero = vecNegative(vecPivot);

    // Translation matrix to and from the zero vector
    mat4x4 matPivotP = matMakeTranslate( vecPivot  );
    mat4x4 matPivotS = matMakeTranslate( vecToZero );

    // Weird. should be matP * matRot * matS
	// But matS * matRot * matP works, maybe its bcs hot mat * vec is done
    matrix = matMultiplyMatrix(matRot,     matPivotP);
    matrix = matMultiplyMatrix(matPivotS,  matrix);

    return matrix;
}