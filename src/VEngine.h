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
#include <psp2/gxm.h> 
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
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <cstdio>

// Include the matrix and vector structs
#include "vector_matrix.h"

// For timer
#include <chrono>
#include <ctime>

// For some quick maths
#include <math.h>
// [TODO]
#include <algorithm>

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
        vec3d vecCamera;
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
            vita2d_set_clear_color(AMBER);
            font = vita2d_load_font_mem(basicfont, basicfont_size);
            #endif

            // init the mat_proj values [RE]
            matProjection.m[0][0] = fAspectRatio * fFovRad;
            matProjection.m[1][1] = fFovRad;
            matProjection.m[2][2] = fFar / (fFar - fNear);
            matProjection.m[3][2] = (-fFar * fNear) / (fFar - fNear);
            matProjection.m[2][3] = 1.0f;
            matProjection.m[3][3] = 0.0f;

            // Set light location
            vecLight = vec3d(0.0f, 0.0f, -1.0f);
        };
        // do it later
        ~VEngine(){
            // Release drawing stuff
            #ifdef PSVITA
            vita2d_init();
            vita2d_free_font(font);

            // Call exit
            sceKernelExitProcess(0);
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
        void draw_mesh(mesh&);
    private:
        // A while loop basically
        // maybe do some extra stuff like taking input
        void ve_clock();
};


#endif//_VENGINE_