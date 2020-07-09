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
	sprintf(buff, "FPS: %6.3f", 99.0f);

    float fElapsedTime = 0.0f;

    while(bClockOn){
        vita2d_start_drawing();
		vita2d_clear_screen();
        
        // Call update function
        update(fElapsedTime);

		vita2d_font_draw_text(font, 20, 20, WHITE, 11, "Hello World!");
		vita2d_font_draw_text(font, 20, 60, WHITE, 11, buff);

		vita2d_end_drawing();
		vita2d_swap_buffers();
    }

}
