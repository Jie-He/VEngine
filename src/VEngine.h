// A simple 3D Engine, mostly from online resources
#ifndef _VENGINE_
#define _VENGINE_
#include <psp2/kernel/processmgr.h>

// Some default C++ stuff
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <cstdio>

// Include the matrix and vector structs
#include "vector_matrix.h"

// For drawing
#include <vita2d.h>

// For timer
#include <chrono>
#include <ctime>

// For some quick maths
#include <math.h>

// Define some colours
#define BLACK       RGBA8(0x00, 0x00, 0x00, 0xFF)
#define WHITE       RGBA8(0xFF, 0xFF, 0xFF, 0xFF)
#define CRIMSON     RGBA8(0xFF, 0x24, 0x00, 0xFF)
#define SEAGREEN    RGBA8(0x54, 0xFF, 0x9F, 0xFF)
#define AMBER       RGBA8(0xFF, 0xBF, 0x00, 0xFF)
#define BLUE        RGBA8(0x03, 0x1C, 0xFC, 0xFF)

// font buffer
extern unsigned int basicfont_size;
extern unsigned char basicfont[];

class VEngine{
    public:
        // World information

        // Control input
        bool bClockOn = false;
        vita2d_font *font;

        // Resolution
        const int SCREEN_WIDTH = 960;
        const int SCREEN_HEIGHT= 544;

        // Perspective stuff
        float fNear         = 0.1f;
        float fFar          = 100.0f;
        float fFov          = 90.0f;
        float fAspectRatio  = (float)(SCREEN_HEIGHT/SCREEN_WIDTH);
        float fFovRad       = 1.0f / tanf( (fFov * 0.5f / 180.0f * 3.14159f) );

        mat4x4 matProjection;

        // Could pass: [RE]
        //  + world colour, 
        //  + all the perspective stuff
        VEngine(){
            // do some default initialisation
            // Initialise 
            vita2d_init();
            vita2d_set_clear_color(AMBER);
            font = vita2d_load_font_mem(basicfont, basicfont_size);

            // init the mat_proj values [RE]
            matProjection.m[0][0] = fAspectRatio * fFovRad;
            matProjection.m[1][1] = fFovRad;
            matProjection.m[2][2] = fFar / (fFar - fNear);
            matProjection.m[3][2] = (-fFar * fNear) / (fFar - fNear);
            matProjection.m[2][3] = 1.0f;
            matProjection.m[3][3] = 0.0f;
        };
        // do it later
        // ~VEngine();

        void start();   // set clock  on and call clock
        void stop();    // set clock off and call clock
        void destory(); // call this last to release memory?

        // Drawing stuff
        void draw_triangle(triangle&);
        void draw_mesh(mesh&);

        // Override this function to do whatever stuff
        virtual void onCreate(){};
        virtual void update(float fElapsedTime){};
    private:
        // A while loop basically
        // maybe do some extra stuff like taking input
        void ve_clock();
};


#endif//_VENGINE_