#include "VEngine.h"

void VEngine::start(){
    bClockOn = true;
    ve_clock();
}

void VEngine::stop(){
    bClockOn = false;
    ve_clock();
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

        // Clear screen 
        #ifdef PSVITA       
        vita2d_start_drawing();
		vita2d_clear_screen();
        #endif

        // OPENCV CLEAR SCREEN
        #ifdef OPENCV
        canvas = world_colour;
        #endif
        
        // Call update function
        update(fElapsedTime);		


        // FPS info
        sprintf(buff, "FPS: %6.3f", fFPS);
        #ifdef PSVITA
        vita2d_font_draw_text(font, 20, 20, WHITE, 11, buff);
        vita2d_end_drawing();
		vita2d_swap_buffers();
        #endif

        // OPENCV DRAW FPS INFO
        #ifdef OPENCV
        cv::putText(canvas, buff, cv::Point(10, 20),
                    cv::FONT_HERSHEY_DUPLEX,
                    0.5, CV_RGB(255, 255, 255), 1);
        // OPENCV DRAW SCREEN && WAIT
        imshow("VEngine", canvas);
        keypress = cv::waitKey(1);
        #endif
    }

}

// Draw a single mesh [RE]
void VEngine::draw_mesh(mesh& mh){
    for (auto tri : mh.tris){
        triangle triProjected;
        triProjected.p[0] = matMultiplyVector(matProjection, triProjected.p[0]);
        triProjected.p[1] = matMultiplyVector(matProjection, triProjected.p[1]);
        triProjected.p[2] = matMultiplyVector(matProjection, triProjected.p[2]);

        draw_triangle(triProjected);

    }
}

// Drawing a triangle
void VEngine::draw_triangle(triangle& tri){
    // draw the three lines
    
    #ifdef PSVITA
    vita2d_draw_line(tri.p[0].x, tri.p[0].y,
					 tri.p[1].x, tri.p[1].y, CRIMSON);
    
    vita2d_draw_line(tri.p[0].x, tri.p[0].y,
					 tri.p[2].x, tri.p[2].y, BLUE);
    
    vita2d_draw_line(tri.p[2].x, tri.p[2].y,
					 tri.p[1].x, tri.p[1].y, SEAGREEN);
    #endif

    // OPENCV DRAW TRIANGLE
    #ifdef OPENCV
    cv::line(canvas, cv::Point(tri.p[0].x, tri.p[0].y),
                     cv::Point(tri.p[1].x, tri.p[1].y),
                     cv::Scalar(255,0,0), 1);

    cv::line(canvas, cv::Point(tri.p[2].x, tri.p[2].y),
                     cv::Point(tri.p[1].x, tri.p[1].y),
                     cv::Scalar(0,255,0), 1);

    cv::line(canvas, cv::Point(tri.p[0].x, tri.p[0].y),
                     cv::Point(tri.p[2].x, tri.p[2].y),
                     cv::Scalar(0,0,255), 1);       
    #endif
}