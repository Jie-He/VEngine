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

// For drawing
#include <vita2d.h>

// For timer
#include <chrono>
#include <ctime>

// For some quick maths
#include <math.h>

// Define some colours
#define BLACK RGBA8(0, 0, 0, 0xFF)
#define WHITE RGBA8(0xFF, 0xFF, 0xFF, 0xFF)
#define CRIMSON RGBA8(0xFF, 0x24, 0, 0xFF)
#define SEAGREEN RGBA8(0x54, 0xFF, 0x9F, 0xFF)
#define AMBER RGBA8(0xFF, 0xBF, 0, 0xFF)

// font buffer
extern unsigned int basicfont_size;
extern unsigned char basicfont[];

// Quick 2D coord struct
struct vec2d{
    float x, y;
};

// 3D coord struct
struct vec3d{
    float x, y, z;
    float w;
};

struct triangle{
    vec3d vertex[3];
    // A base colour for this face?
};

struct mesh{
    std::vector<triangle> tris; 
};

class VEngine{
    public:
        // World information

        // Control input
        bool bClockOn = false;
        vita2d_font *font;

        VEngine(){
            // do some default initialisation
            
            // Initialise 
            vita2d_init();
            vita2d_set_clear_color(BLACK);
            font = vita2d_load_font_mem(basicfont, basicfont_size);
        };

        void start();   // set clock  on and call clock
        void stop();    // set clock off and call clock
        void destory(); // call this last to release memory?
        // Override this function to do whatever stuff
        virtual void update(float fElapsedTime){};
    private:
        // A while loop basically
        // maybe do some extra stuff like taking input
        void ve_clock();

};


#endif//_VENGINE_