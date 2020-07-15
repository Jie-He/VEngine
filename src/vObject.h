// ============================================================
//  Header file for definition of a vObject
//  Main purpose is to keep track of the 
//  three directional vectors which determines where a 
//  object is currently looking at.
//  
//  The three (normalised) vectors defines the following:
//   + Forward direction
//   + Upward  direction
//   + Right   direction
// ============================================================

#ifndef _VOBJECT_
#define _VOBJECT_
#include "vector_matrix.h"

class vObject{
    protected:
        // Object location
        vec3d vecLocation;
        // All with respect to ZERO
        vec3d vecForward;
        vec3d vecVertical;
        vec3d vecHorizontal;
        // Matrix for rotation at pivot
        mat4x4 matRotPivot;
        // Active status. if not consider this object deleted
        bool bActive;
    public:
        vObject(){
            // set the default vectors to axis
            // -Z is depth, -Y is up
            vecLocation  = vec3d();
            vecForward   = vec3d(0.0f,  0.0f, 1.0f);
            vecVertical  = vec3d(0.0f,  1.0f, 0.0f);
            vecHorizontal= vecCrossProduct(vecVertical, vecForward);
            bActive = true;
        }

        // Translate the location vector and all the directional ones.
        void ApplyTranslation(vec3d& vecTrans);
        // Rotate the directional vectors around a pivot.
        // Set pivot to object location for self rotation.
        void ApplyRotation(mat4x4& matRot, vec3d& vecPivot);
        // Given a vector as a target.
        // All the directional vectors will be updated so that
        // vecForward points at the given vector
        void PointAt(vec3d& vecTarget);
        // should pass control info too.
        // All objects should override this
        virtual void update(float){};

        // Getters
        vec3d& getVecLocation()  { return vecLocation;  };
        vec3d& getVecForward()   { return vecForward;   };
        vec3d& getVecVertical()  { return vecVertical;  };
        vec3d& getVecHorizontal(){ return vecHorizontal;};
        bool isActive() { return bActive; };
};

#endif//_VOBJECT_