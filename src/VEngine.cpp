#include "VEngine.h"

void VEngine::start(){
    bClockOn = true;
    ve_clock();
}

void VEngine::stop(){
    bClockOn = false;
    ve_clock();
}

// Before wasting this object. clear some free memory
void VEngine::destory(){

    // Release drawing stuff
    vita2d_init();
    vita2d_free_font(font);
}

// Main clock
void VEngine::ve_clock(){
    
    // For FPS information
    char buff[16];
	

    // maybe get an clock ticking?
	auto cTimeStamp = std::chrono::system_clock::now();
    auto cTimeCurrent = std::chrono::system_clock::now();

    float fElapsedTime = 0.0f; // in seconds?
    float fFPS = 0.0f;

    while(bClockOn){
        // calc elasped time since last loop
        cTimeCurrent = std::chrono::high_resolution_clock::now();
        // elapsed in seconds
        fElapsedTime = (std::chrono::duration<float>(cTimeCurrent - cTimeStamp).count()); 
        // reset time stamp
        cTimeStamp = cTimeCurrent;
        // Conversion && FPS
        fFPS = 1 / fElapsedTime;

        // init the drawing 
        vita2d_start_drawing();
		vita2d_clear_screen();
        
        // Call update function
        update(fElapsedTime);		


        // FPS info
        sprintf(buff, "FPS: %6.3f", fFPS);
        vita2d_font_draw_text(font, 20, 20, WHITE, 11, buff);

		vita2d_end_drawing();
		vita2d_swap_buffers();
    }

}

// Draw a single mesh [RE]
void VEngine::draw_mesh(mesh& mh){
    for (auto tri : mh.tris){
        triangle triProjected;
        triProjected.p[0] = Matrix_MultiplyVector(matProjection, triProjected.p[0]);
        triProjected.p[1] = Matrix_MultiplyVector(matProjection, triProjected.p[1]);
        triProjected.p[2] = Matrix_MultiplyVector(matProjection, triProjected.p[2]);

        draw_triangle(triProjected);

    }
}

// Drawing a triangle
void VEngine::draw_triangle(triangle& t){
    // draw the three lines
    vita2d_draw_line(t.p[0].x, t.p[0].y,
					 t.p[1].x, t.p[1].y, CRIMSON);
    
    vita2d_draw_line(t.p[0].x, t.p[0].y,
					 t.p[2].x, t.p[2].y, BLUE);
    
    vita2d_draw_line(t.p[2].x, t.p[2].y,
					 t.p[1].x, t.p[1].y, SEAGREEN);
}