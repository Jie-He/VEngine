// ============================================================
// A simple 3D Engine, mostly from online resources.
// Code is primarily from JAVIDX9 

// Main platform is for PS Vita console with VitaSDK library.
// Due to Vita3K emulator not working very well, 
// added OpenCV render to test algorithms before deploy to Vita

// This code is mainly for personal learning purposes.
// ============================================================
#ifndef _VENGINE_
#define _VENGINE_

// VITA stuff
#ifdef PSVITA
#include <psp2/kernel/processmgr.h>
#include <psp2/ctrl.h>
// For drawing
#include <vita2d.h>
#endif 

// OPENCV temp
// OpenCV things
#ifdef OPENCV
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#endif

// Some default C++ stuff
#include <iostream>
#include <sstream>
#include <vector>
#include <cstdio>

// Include the matrix and vector structs
#include "vector_matrix.h"

// For timer
#include <chrono>

// For some quick maths
#include <math.h>
// [TODO]
#include <algorithm>
#include <list>

// Define some colours
#ifdef PSVITA
#define BLACK       RGBA8(0x00, 0x00, 0x00, 0xFF)
#define WHITE       RGBA8(0xFF, 0xFF, 0xFF, 0xFF)
#define CRIMSON     RGBA8(0xFF, 0x24, 0x00, 0xFF)
#define SEAGREEN    RGBA8(0x54, 0xFF, 0x9F, 0xFF)
#define AMBER       RGBA8(0xFF, 0xBF, 0x00, 0xFF)
#define BLUE        RGBA8(0x03, 0x1C, 0xFC, 0xFF)

// font buffer
extern unsigned int basicfont_size;
extern unsigned char basicfont[];
#endif

struct Camera{
    // Location 
    vec3d location = vec3d();
    vec3d lookAtDir= vec3d(0.0f, 0.0f, 1.0f);
    vec3d vecUp    = vec3d(0.0f, 1.0f, 0.0f);
    vec3d vecRight = vec3d(1.0f, 0.0f, 0.0f);
    // [TODO]: Add constructor

    // Viewing Matrix
    mat4x4 matCamera;

    void pointAt(vec3d& target){
        // Need to update the look at things
        
        // Calc new forward direction
        lookAtDir = target - location;
        lookAtDir = vecNormalise(lookAtDir);

        // Calc new up direction
        vec3d temp = vecUp;
        vecUp = lookAtDir * (vecUp.dot(lookAtDir));
        vecUp = temp - vecUp;
        vecUp = vecNormalise(vecUp);

        // Cross product for new right direction
        vecRight = vecCrossProduct(vecUp, lookAtDir);

        // Construct new matrix
        matCamera = matMakeTranslate(location);
        matCamera.m[0][0] = vecRight.x;	    matCamera.m[0][1] = vecRight.y;	    matCamera.m[0][2] = vecRight.z;	
        matCamera.m[1][0] = vecUp.x;		matCamera.m[1][1] = vecUp.y;		matCamera.m[1][2] = vecUp.z;		
        matCamera.m[2][0] = lookAtDir.x;	matCamera.m[2][1] = lookAtDir.y;	matCamera.m[2][2] = lookAtDir.z;
        
        //matCamera= matPointAt(location, target, vecUp);
		matCamera= matQuickInverse(matCamera);
    }

    void ApplyRotation(mat4x4& m){
        // update the 3 vectors
        lookAtDir = matMultiplyVector(m, lookAtDir);
        vecUp     = matMultiplyVector(m, vecUp);
        vecRight  = matMultiplyVector(m, vecRight);
    }
};

class VEngine{
    public:
        // Control input
        bool bClockOn = false;
        #ifdef PSVITA
        // Fonts to render with
        vita2d_font *font;
        // Get store controller input
        SceCtrlData   ctrl;
        #endif

        // Resolution
        const int SCREEN_WIDTH = 960;
        const int SCREEN_HEIGHT= 544;

        // World information
        // Camera
        Camera camMain;
        // Lighting
        vec3d vecLight;
        // OPENCV: canvas, color, input
        #ifdef OPENCV
        cv::Mat canvas          = cv::Mat::zeros(SCREEN_HEIGHT, SCREEN_WIDTH, CV_8UC3);
        cv::Scalar world_colour = cv::Scalar(0, 0, 0);
        char keypress;
        #endif
        // VITA CTRL

        // Perspective stuff
        float fNear         = 0.1f;
        float fFar          = 1000.0f;
        float fFov          = 90.0f;
        float fAspectRatio  = ((float)SCREEN_HEIGHT/(float)SCREEN_WIDTH);
        float fFovRad       = 1.0f / tanf( (fFov * 0.5f / 180.0f * 3.14159f) );

        mat4x4 matProjection;
        // Could pass: [RE]
        //  + world colour, 
        VEngine(){
            // do some default initialisation
            // Initialise 
            #ifdef PSVITA
            vita2d_init();
            vita2d_set_clear_color(BLACK);
            font = vita2d_load_font_mem(basicfont, basicfont_size);
            sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG_WIDE);
            #endif

            // init the mat_proj values [RE]
            matProjection.m[0][0] = fAspectRatio * fFovRad;
            matProjection.m[1][1] = fFovRad;
            matProjection.m[2][2] = fFar / (fFar - fNear);
            matProjection.m[3][2] = (-fFar * fNear) / (fFar - fNear);
            matProjection.m[2][3] = 1.0f;
            matProjection.m[3][3] = 0.0f;

            // Set light location
            vecLight = vec3d(0.0f, -10.0f, -10.0f);
        };
        // do it later
        ~VEngine(){
            // Release drawing stuff
            #ifdef PSVITA
            vita2d_fini();
            vita2d_free_font(font);
            #endif
        };

        void start();   // set clock  on and call clock
        void stop();    // set clock off and call clock
        //void destory(); // call this last to release memory?

        // Override this function to do whatever stuff
        virtual void onCreate(){};
        virtual void update(float fElapsedTime){};

        // Drawing stuff
        void draw_triangle(triangle&);
        void fill_triangle(triangle&, int, int, int);
        void draw_scene(std::vector<mesh>&);
    private:
        // A while loop basically
        // maybe do some extra stuff like taking input
        void ve_clock();
};


#endif//_VENGINE_