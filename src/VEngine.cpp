#include "VEngine.h"

void VEngine::start(){
    bClockOn = true;
    onCreate();
    ve_clock();
}

void VEngine::stop(){
    bClockOn = false;
    //ve_clock();
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

            // Temp break program button ( Select button )
            if (ctrl.buttons & SCE_CTRL_SELECT) break;	
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
            // Print stick info.. since its not working
            sprintf(buff, "LX: %d, LY: %d", ctrl.lx, ctrl.ly);
            vita2d_font_draw_text(font, 20, 60, WHITE, 11, buff);
            sprintf(buff, "RX: %d, RY: %d", ctrl.rx, ctrl.ry);
            vita2d_font_draw_text(font, 20, 80, WHITE, 11, buff);

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
            // Exit when esc key pressed
            if (keypress == 27) break;
        #endif
    }

}

vec3d vecIntersectPlane(vec3d &plane_p, vec3d &plane_n, vec3d &lineStart, vec3d &lineEnd){
    plane_n = vecNormalise(plane_n);
    float plane_d = - plane_n.dot(plane_p);
    float ad = lineStart.dot(plane_n);
    float bd = lineEnd.dot(plane_n);
    float t = (-plane_d - ad) / (bd - ad);
    vec3d lineStartToEnd = lineEnd - lineStart;
    vec3d lineToIntersect = lineStartToEnd * t;
    return (lineStart + lineToIntersect);
}

int TriangleClipAgainstPlane(vec3d plane_p, vec3d plane_n, triangle &in_tri, triangle &out_tri1, triangle &out_tri2)
	{
		// Make sure plane normal is indeed normal
		plane_n = vecNormalise(plane_n);

		// Return signed shortest distance from point to plane, plane normal must be normalised
		auto dist = [&](vec3d &p)
		{
			vec3d n = vecNormalise(p);
            // TODO is it N? or P
			return (plane_n.dot(p) - plane_n.dot(plane_p));
		};

		// Create two temporary storage arrays to classify points either side of plane
		// If distance sign is positive, point lies on "inside" of plane
		vec3d* inside_points[3];  int nInsidePointCount = 0;
		vec3d* outside_points[3]; int nOutsidePointCount = 0;

		// Get signed distance of each point in triangle to plane
		float d0 = dist(in_tri.p[0]);
		float d1 = dist(in_tri.p[1]);
		float d2 = dist(in_tri.p[2]);

		if (d0 >= 0) { inside_points[nInsidePointCount++] = &in_tri.p[0]; }
		else { outside_points[nOutsidePointCount++] = &in_tri.p[0]; }
		if (d1 >= 0) { inside_points[nInsidePointCount++] = &in_tri.p[1]; }
		else { outside_points[nOutsidePointCount++] = &in_tri.p[1]; }
		if (d2 >= 0) { inside_points[nInsidePointCount++] = &in_tri.p[2]; }
		else { outside_points[nOutsidePointCount++] = &in_tri.p[2]; }

		// Now classify triangle points, and break the input triangle into 
		// smaller output triangles if required. There are four possible
		// outcomes...

		if (nInsidePointCount == 0)
			return 0; // No returned triangles are valid


		if (nInsidePointCount == 3){
			// All points lie on the inside of plane, so do nothing
			out_tri1 = in_tri;
			return 1;
		}

		if (nInsidePointCount == 1 && nOutsidePointCount == 2){
			// Triangle should be clipped. As two points lie outside
			// the plane, the triangle simply becomes a smaller triangle

            out_tri1.fGrayScale = in_tri.fGrayScale;
			out_tri1.p[0] = *inside_points[0];

			// but the two new points are at the locations where the 
			// original sides of the triangle (lines) intersect with the plane
			out_tri1.p[1] = vecIntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[0]);
			out_tri1.p[2] = vecIntersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[1]);
			return 1;
		}

		if (nInsidePointCount == 2 && nOutsidePointCount == 1){
			// Triangle should be clipped. As two points lie inside the plane,
			// the clipped triangle becomes a "quad". Fortunately, we can
			// represent a quad with two new triangles
            
            out_tri1.fGrayScale = in_tri.fGrayScale;
            out_tri2.fGrayScale = in_tri.fGrayScale;
			// The first triangle consists of the two inside points and a new
			// point determined by the location where one side of the triangle
			// intersects with the plane
			out_tri1.p[0] = *inside_points[0];
			out_tri1.p[1] = *inside_points[1];
			out_tri1.p[2] = vecIntersectPlane (plane_p, plane_n, *inside_points[0], *outside_points[0]);

			// The second triangle is composed of one of he inside points, a
			// new point determined by the intersection of the other side of the 
			// triangle and the plane, and the newly created point above
			out_tri2.p[0] = *inside_points[1];
			out_tri2.p[1] = out_tri1.p[2];
			out_tri2.p[2] = vecIntersectPlane(plane_p, plane_n, *inside_points[1], *outside_points[0]);

			return 2;
		}
	}

// Draw a single mesh [RE]
void VEngine::draw_scene(std::vector<mesh>& sceMesh){
    std::vector<triangle> vecTrianglesToRaster;

    for (mesh& mh : sceMesh){
        for (auto& tri : mh.tris){
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
            vec3d temp = tri.p[0] - camMain.location;
            if (normal.dot(temp) < 0.0f){
                triangle triProjected, triTranslate, triScale;
                // Project to camera view first
                triProjected.p[0] = matMultiplyVector(camMain.matCamera, tri.p[0]);
                triProjected.p[1] = matMultiplyVector(camMain.matCamera, tri.p[1]);
                triProjected.p[2] = matMultiplyVector(camMain.matCamera, tri.p[2]);

                int nClippedTriangles = 0;
                triangle clipped[2];

                nClippedTriangles = TriangleClipAgainstPlane({0.0f, 0.0f, 0.1f}, {0.0f, 0.0f, 1.0f},
                                                triProjected, clipped[0], clipped[1]);

                for (int n = 0; n < nClippedTriangles; n++){
                     // Project the vec3d with the projection matrix
                    triProjected.p[0] = matMultiplyVector(matProjection, clipped[n].p[0]);
                    triProjected.p[1] = matMultiplyVector(matProjection, clipped[n].p[1]);
                    triProjected.p[2] = matMultiplyVector(matProjection, clipped[n].p[2]);

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

    for (auto &triToRaster : vecTrianglesToRaster){
        // Clip triangles against all four screen edges, this could yield
        triangle clipped[2];
        std::list<triangle> listTriangles;

        // Add initial triangle
        listTriangles.push_back(triToRaster);
        int nNewTriangles = 1;

        for (int p = 0; p < 4; p++){
            int nTrisToAdd = 0;
            while (nNewTriangles > 0)
            {
                // Take triangle from front of queue
                triangle test = listTriangles.front();
                listTriangles.pop_front();
                nNewTriangles--;

                // Clip it against a plane. We only need to test each 
                // subsequent plane, against subsequent new triangles
                // as all triangles after a plane clip are guaranteed
                // to lie on the inside of the plane. I like how this
                // comment is almost completely and utterly justified
                switch (p)
                {
                case 0:	nTrisToAdd = TriangleClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
                case 1:	nTrisToAdd = TriangleClipAgainstPlane({ 0.0f, (float)(SCREEN_HEIGHT) - 1, 0.0f }, { 0.0f, -1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
                case 2:	nTrisToAdd = TriangleClipAgainstPlane({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
                case 3:	nTrisToAdd = TriangleClipAgainstPlane({ (float)(SCREEN_WIDTH - 1), 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
                }

                // Clipping may yield a variable number of triangles, so
                // add these new ones to the back of the queue for subsequent
                // clipping against next planes
                for (int w = 0; w < nTrisToAdd; w++)
                    listTriangles.push_back(clipped[w]);
            }
            nNewTriangles = listTriangles.size();
        }


        for (auto &t : listTriangles){
            fill_triangle(t, t.fGrayScale, t.fGrayScale, t.fGrayScale);   
            draw_triangle(t);
        }
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