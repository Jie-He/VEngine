#include "VEngine.h"

void VEngine::start(){
    bClockOn = true;
    onCreate();
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

        // Get controller input
        sceCtrlPeekBufferPositive(0, &ctrl, 1);

        // Temp break program button ( R Trigger )
        if (ctrl.buttons & SCE_CTRL_RTRIGGER) break;	

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

    std::vector<triangle> vecTrianglesToRaster;

    for (auto tri : mh.tris){
        // Normal calculation
        vec3d normal, line1, line2;
        // Get lines from either side of triangle
        line1 = tri.p[1] - tri.p[0];
        line2 = tri.p[2] - tri.p[0];
        
        // Take crossproduct of lines to get normal to triangle surface
        normal = vecCrossProduct(line1, line2);
        // Normalise a normal
        normal = vecNormalise(normal);

        // Only draw it if its visible from view
        vec3d temp = tri.p[0] - vecCamera;
        if (normal.dot(temp) < 0){
            triangle triProjected, triTranslate, triScale;
            // Project the vec3d with the projection matrix
            triProjected.p[0] = matMultiplyVector(matProjection, tri.p[0]);
            triProjected.p[1] = matMultiplyVector(matProjection, tri.p[1]);
            triProjected.p[2] = matMultiplyVector(matProjection, tri.p[2]);

            // Temp, translating to the centre of the screen (0,0)
            vec3d vecTranslate(1.0f, 1.0f, 0.0f);
            triTranslate.p[0] = triProjected.p[0] + vecTranslate;
            triTranslate.p[1] = triProjected.p[1] + vecTranslate;
            triTranslate.p[2] = triProjected.p[2] + vecTranslate;

            // Scale into view
            vec3d vecScale(0.5f * (float)SCREEN_WIDTH, 0.5f * (float)SCREEN_HEIGHT, 1.0f);
            triScale.p[0] = triTranslate.p[0] * vecScale;
            triScale.p[1] = triTranslate.p[1] * vecScale;
            triScale.p[2] = triTranslate.p[2] * vecScale;

            // Simple light shading
            vecLight = vecNormalise(vecLight);
            float ls = vecLight.dot(normal) * 255;
            triScale.fGrayScale = ls;
            
            vecTrianglesToRaster.push_back(triScale);
            //fill_triangle(triScale, ls, ls, ls);
            //draw_triangle(triScale);
        }
    }

    // sort the faces by distance, :. Painter's Method
    // Change to Depth buffer later
    std::sort(vecTrianglesToRaster.begin(), vecTrianglesToRaster.end(),
         [](triangle &t1, triangle &t2){
             float z1 = (t1.p[0].z + t1.p[1].z + t1.p[2].z) / 3.0f;
             float z2 = (t2.p[0].z + t2.p[1].z + t2.p[2].z) / 3.0f;
             
             return z1 > z2;
         });

    for (auto tri : vecTrianglesToRaster){
        fill_triangle(tri, tri.fGrayScale, tri.fGrayScale, tri.fGrayScale);
        draw_triangle(tri);
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
                     cv::Scalar(255,0,0), 2);

    cv::line(canvas, cv::Point(tri.p[2].x, tri.p[2].y),
                     cv::Point(tri.p[1].x, tri.p[1].y),
                     cv::Scalar(0,255,0), 2);

    cv::line(canvas, cv::Point(tri.p[0].x, tri.p[0].y),
                     cv::Point(tri.p[2].x, tri.p[2].y),
                     cv::Scalar(0,0,255), 2);
    #endif
}

void VEngine::fill_triangle(triangle& tri, int R=255, int G=255, int B=255){
    #ifdef PSVITA
    // [TODO]
    #endif

    #ifdef OPENCV
    std::vector<cv::Point> pts = {  cv::Point(tri.p[0].x, tri.p[0].y),
                                    cv::Point(tri.p[1].x, tri.p[1].y),
                                    cv::Point(tri.p[2].x, tri.p[2].y)};
    cv::fillPoly(canvas, pts, cv::Scalar(B, G, R));   
    #endif
}