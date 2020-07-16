#include "vector_matrix.h"

// Operation on vector
vec3d vecCrossProduct(vec3d& v1, vec3d& v2){
    vec3d v;
    v.x = v1.y * v2.z - v1.z * v2.y;
    v.y = v1.z * v2.x - v1.x * v2.z;
    v.z = v1.x * v2.y - v1.y * v2.x;
    return v;
}

vec3d vecNormalise(vec3d& v){
    float l = v.length();
    return vec3d(v.x / l, v.y / l, v.z / l);
}

vec3d vecNegative(vec3d& v){
    vec3d v1;
    v1.x = -v.x;
    v1.y = -v.y;
    v1.z = -v.z;
    // Not w at this point
    return v1;
}

vec3d matMultiplyVector(mat4x4 &m, vec3d &i){
    vec3d v;
    v.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + i.w * m.m[3][0];
    v.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + i.w * m.m[3][1];
    v.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + i.w * m.m[3][2];
    v.w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + i.w * m.m[3][3];
    
    // for now [RE]
    if (v.w != 0)
    {   v.x /= v.w; v.y /= v.w; v.z /= v.w; }

    return v;
}

mat4x4 matIdentity(){
    mat4x4 v;
    for (int i = 0; i < 4; i++)
        v.m[i][i] = 1.0f;

    return v;
}

mat4x4 matMakeTranslate(vec3d& vecTrans){
    mat4x4 matrix = matIdentity();
    matrix.m[3][0]  = vecTrans.x;
    matrix.m[3][1]  = vecTrans.y;
    matrix.m[3][2]  = vecTrans.z;
    return matrix;
}

mat4x4 matPointAt(vec3d &pos, vec3d &target, vec3d &up){
    // Calc new forward direction
    vec3d vecForward = target - pos;
    vecForward = vecNormalise(vecForward);

    // Calc new up direction
    vec3d vecUp = vecForward * (up.dot(vecForward));
    vecUp = up - vecUp;
    vecUp = vecNormalise(vecUp);

    // Cross product for new right direction
    vec3d vecRight = vecCrossProduct(vecUp, vecForward);

    // Construct new matrix
    mat4x4 matrix = matMakeTranslate(pos);
    matrix.m[0][0] = vecRight.x;	matrix.m[0][1] = vecRight.y;	matrix.m[0][2] = vecRight.z;	
    matrix.m[1][0] = vecUp.x;		matrix.m[1][1] = vecUp.y;		matrix.m[1][2] = vecUp.z;		
    matrix.m[2][0] = vecForward.x;	matrix.m[2][1] = vecForward.y;	matrix.m[2][2] = vecForward.z;
    return matrix;
}

// Only for rotation/translation matrix where there was 3 bloody 0 on the last column
mat4x4 matQuickInverse(mat4x4 &m){
    mat4x4 matrix;
    matrix.m[0][0] = m.m[0][0]; matrix.m[0][1] = m.m[1][0]; matrix.m[0][2] = m.m[2][0]; matrix.m[0][3] = 0.0f;
    matrix.m[1][0] = m.m[0][1]; matrix.m[1][1] = m.m[1][1]; matrix.m[1][2] = m.m[2][1]; matrix.m[1][3] = 0.0f;
    matrix.m[2][0] = m.m[0][2]; matrix.m[2][1] = m.m[1][2]; matrix.m[2][2] = m.m[2][2]; matrix.m[2][3] = 0.0f;
    matrix.m[3][0] = -(m.m[3][0] * matrix.m[0][0] + m.m[3][1] * matrix.m[1][0] + m.m[3][2] * matrix.m[2][0]);
    matrix.m[3][1] = -(m.m[3][0] * matrix.m[0][1] + m.m[3][1] * matrix.m[1][1] + m.m[3][2] * matrix.m[2][1]);
    matrix.m[3][2] = -(m.m[3][0] * matrix.m[0][2] + m.m[3][1] * matrix.m[1][2] + m.m[3][2] * matrix.m[2][2]);
    matrix.m[3][3] = 1.0f;
    return matrix;
}

mat4x4 matMakeRotationX(float fAngleRad)
{
    mat4x4 matrix;
    matrix.m[0][0] = 1.0f;
    matrix.m[1][1] = cosf(fAngleRad);
    matrix.m[1][2] = sinf(fAngleRad);
    matrix.m[2][1] = -sinf(fAngleRad);
    matrix.m[2][2] = cosf(fAngleRad);
    matrix.m[3][3] = 1.0f;
    return matrix;
}

mat4x4 matMakeRotationY(float fAngleRad)
{
    mat4x4 matrix;
    matrix.m[0][0] = cosf(fAngleRad);
    matrix.m[0][2] = sinf(fAngleRad);
    matrix.m[2][0] = -sinf(fAngleRad);
    matrix.m[1][1] = 1.0f;
    matrix.m[2][2] = cosf(fAngleRad);
    matrix.m[3][3] = 1.0f;
    return matrix;
}

mat4x4 matMakeRotationZ(float fAngleRad)
{
    mat4x4 matrix;
    matrix.m[0][0] = cosf(fAngleRad);
    matrix.m[0][1] = sinf(fAngleRad);
    matrix.m[1][0] = -sinf(fAngleRad);
    matrix.m[1][1] = cosf(fAngleRad);
    matrix.m[2][2] = 1.0f;
    matrix.m[3][3] = 1.0f;
    return matrix;
}

mat4x4 matMakeScale(float fScale, vec3d& vecPivot){
    mat4x4 matrix;
    matrix.m[0][0] = fScale;
    matrix.m[1][1] = fScale;
    matrix.m[2][2] = fScale;
    matrix.m[3][3] = 1.0f; // might be safe to keep this as 1
    vec3d vecToZero(0.0f, 0.0f, 0.0f);
    vecToZero = vecToZero - vecPivot;

    // Translation matrix to and from the zero vector
    mat4x4 matPivotP = matMakeTranslate( vecPivot  );
    mat4x4 matPivotS = matMakeTranslate( vecToZero );

    // Weird. should be matP * matRot * matS
	// But matS * matRot * matP works, maybe its bcs hot mat * vec is done
    matrix = matMultiplyMatrix(matrix,     matPivotP);
    matrix = matMultiplyMatrix(matPivotS,  matrix);

    return matrix;
}

mat4x4 matMakeRotationPivot(mat4x4& matRot, vec3d& vecPivot){
    mat4x4 matrix;
    vec3d vecToZero(0.0f, 0.0f, 0.0f);
    vecToZero = vecToZero - vecPivot;

    // Translation matrix to and from the zero vector
    mat4x4 matPivotP = matMakeTranslate( vecPivot  );
    mat4x4 matPivotS = matMakeTranslate( vecToZero );

    // Weird. should be matP * matRot * matS
	// But matS * matRot * matP works, maybe its bcs hot mat * vec is done
    matrix = matMultiplyMatrix(matRot,     matPivotP);
    matrix = matMultiplyMatrix(matPivotS,  matrix);

    return matrix;
}

mat4x4 matMakeRotationAxis(vec3d& vecLoc, vec3d& vecAxe, float fAngleRad){
    mat4x4 matrix = matIdentity();
    vec3d vecAxis = vecNormalise(vecAxe);

    // Preparing... 
    float u(vecAxis.x), v(vecAxis.y), w(vecAxis.z);
    float a( vecLoc.x), b( vecLoc.y), c( vecLoc.z);

    float u2 = u*u;
    float v2 = v*v;
    float w2 = w*w;

    float co = cosf(fAngleRad);
    float nc = 1.0f - co;
    float so = sinf(fAngleRad);

    matrix.m[0][0] = u2+(v2+w2)*co;   matrix.m[1][0] = u*v*nc+w*so;     matrix.m[2][0] = u*w*nc-v*so;
    matrix.m[0][1] = u*v*nc-w*so;     matrix.m[1][1] = v2+(u2+w2)*co;   matrix.m[2][1] = v*w*nc+u*so;
    matrix.m[0][2] = u*w*nc+v*so;     matrix.m[1][2] = v*w*nc-u*so;     matrix.m[2][2] = w2+(u2+v2)*co;
    matrix.m[0][3] = (a*(v2 + w2) - u*(b*v+c*w))*nc + (b*w-c*v)*so;
    matrix.m[1][3] = (b*(u2 + w2) - v*(a*u+c*w))*nc + (c*u-a*w)*so;
    matrix.m[2][3] = (c*(u2 + v2) - w*(a*u+b*v))*nc + (a*v-b*u)*so;
    
    return matrix;
}


mat4x4 matMultiplyMatrix(mat4x4 &m1, mat4x4 &m2)
{
    mat4x4 matrix;
    for (int c = 0; c < 4; c++)
        for (int r = 0; r < 4; r++)
            matrix.m[r][c] = m1.m[r][0] * m2.m[0][c] + m1.m[r][1] * m2.m[1][c] + m1.m[r][2] * m2.m[2][c] + m1.m[r][3] * m2.m[3][c];
    return matrix;
}

mat4x4 matMakeProjection(float fFovRad, float fAspectRatio, float fNear, float fFar){
    mat4x4 matProj;
    matProj.m[0][0] = fAspectRatio * fFovRad;
    matProj.m[1][1] = fFovRad;
    matProj.m[2][2] = fFar / (fFar - fNear);
    matProj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
    matProj.m[2][3] = 1.0f;
    matProj.m[3][3] = 0.0f;
    return matProj;
}