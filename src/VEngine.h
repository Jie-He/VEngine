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

// My classes
#include "vObject.h"
#include "vCamera.h"
#include "vMesh.h"

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
        // Lighting
        vec3d vecLight;
        // OPENCV: canvas, color, input
        #ifdef OPENCV
        cv::Mat canvas          = cv::Mat::zeros(SCREEN_HEIGHT, SCREEN_WIDTH, CV_8UC3);
        cv::Scalar world_colour = cv::Scalar(0, 0, 0);
        char keypress;
        #endif
        // VITA CTRL
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

            // Set light location
            vecLight = vec3d(0.0f, -10.0f, 0.0f);
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
        void fill_triangle(triangle&, vec3d&);
        void draw_scene(vCamera&, std::vector<vMesh>&);
    private:
        // A while loop basically
        // maybe do some extra stuff like taking input
        void ve_clock();
};


#endif//_VENGINE_