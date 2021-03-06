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
    char buff[128];	

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
            canvas = cv::Scalar(world_colour.z, world_colour.y, world_colour.x);
        #endif
        
        // Call update function
        update(fElapsedTime);		

        // FPS info
        sprintf(buff, "FPS: %6.3f", fFPS);
        #ifdef PSVITA
            vita2d_font_draw_text(font, 22, 22, BLACK, 12, buff);
            vita2d_font_draw_text(font, 20, 20, WHITE, 11, buff);
            vita2d_end_drawing();
            vita2d_swap_buffers();
        #endif

        // OPENCV DRAW FPS INFO
        #ifdef OPENCV
            cv::putText(canvas, buff, cv::Point(22, 22),
                cv::FONT_HERSHEY_DUPLEX,
                0.5, CV_RGB(0,0,0), 2);
            cv::putText(canvas, buff, cv::Point(20, 20),
                        cv::FONT_HERSHEY_DUPLEX,
                        0.5, CV_RGB(255, 255, 255), 1);
            // OPENCV DRAW SCREEN && WAIT
            imshow("VEngine", canvas);
            keypress = cv::waitKey(10);
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

		if (nInsidePointCount == 3){
			// All points lie on the inside of plane, so do nothing
			out_tri1 = in_tri;
			return 1;
		}

		if (nInsidePointCount == 1 && nOutsidePointCount == 2){
			// Triangle should be clipped. As two points lie outside
			// the plane, the triangle simply becomes a smaller triangle
            
            out_tri1.colour = in_tri.colour;
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
            
            out_tri1.colour = in_tri.colour;
            out_tri2.colour = in_tri.colour;
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

        // If all three points are outside
		return 0; // No returned triangles are valid

	}

void VEngine::draw_to_frame(vCamera& camMain, std::vector<triangle>& vecTrianglesToRaster, bool outline){
    // sort the faces by distance, :. Painter's Method
    // Change to Depth buffer later
    //#ifdef OPENCV
    std::sort(vecTrianglesToRaster.begin(), vecTrianglesToRaster.end(),
        [](triangle &t1, triangle &t2){
            float z1 = (t1.p[0].z + t1.p[1].z + t1.p[2].z) / 3.0f;
            float z2 = (t2.p[0].z + t2.p[1].z + t2.p[2].z) / 3.0f;
            
            return z1 > z2;
        });
    //#endif

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
                // to lie on the inside of the plane. 
                switch (p)
                {
                case 0:	nTrisToAdd = TriangleClipAgainstPlane({ 0.0f, (float)(camMain.nScreenOY + 1), 0.0f }, { 0.0f, 1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
                case 1:	nTrisToAdd = TriangleClipAgainstPlane({ 0.0f, (float)(camMain.nScreenH + camMain.nScreenOY - 1), 0.0f }, { 0.0f, -1.0f, 0.0f }, test, clipped[0], clipped[1]); break;
                case 2:	nTrisToAdd = TriangleClipAgainstPlane({ (float)(camMain.nScreenOX + 1), 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
                case 3:	nTrisToAdd = TriangleClipAgainstPlane({ (float)(camMain.nScreenW + camMain.nScreenOX - 1), 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, test, clipped[0], clipped[1]); break;
                }

                // Clipping may yield a variable number of triangles, so
                // add these new ones to the back of the queue for subsequent
                // clipping against next planes
                for (int w = 0; w < nTrisToAdd; w++)
                    listTriangles.push_back(clipped[w]);
            }
            nNewTriangles = listTriangles.size();
        }
        
        if(outline){                
            for (auto &t : listTriangles){
                fill_triangle(t, t.colour);   
                draw_triangle(t);
            }
        }else{
            for (auto &t : listTriangles){
                fill_triangle(t, t.colour);   
            }
        }
    }   
}

void VEngine::project_mesh(vCamera& camMain, vec3d& vecScale, vec3d& vecOffset, vec3d& vecTranslate, vMesh& mh, std::vector<triangle>& vecTrianglesToRaster, bool outline){
    for (auto& tri : mh.getTris()){
        // Normal calculation
        vec3d normal, line1, line2;
        // Get lines from either side of triangle
        line1 = tri.p[1] - tri.p[0];
        line2 = tri.p[2] - tri.p[0];
        
        // Take crossproduct of lines to get normal to triangle surface
        normal = vecCrossProduct(line1, line2);
        // Normalise a normal
        normal = vecNormalise(normal);

        // Simple light shading
        float ls = 1.0f;
        if (bLighting){
            vec3d vecToLight = vecLight - tri.p[0];
            vecToLight = vecNormalise(vecToLight);
            ls = std::max(0.2f, normal.dot(vecToLight));
        }
        

        // Only draw it if its visible from view
        vec3d temp = tri.p[0] - camMain.getVecLocation();
        if (normal.dot(temp) < 0.0f){
            triangle triProjected, triTranslate, triScale;
            // Project to camera view first
            triProjected.p[0] = matMultiplyVector(camMain.matCamView, tri.p[0]);
            triProjected.p[1] = matMultiplyVector(camMain.matCamView, tri.p[1]);
            triProjected.p[2] = matMultiplyVector(camMain.matCamView, tri.p[2]);
            triProjected.colour = tri.colour * ls;
            int nClippedTriangles = 0;
            triangle clipped[2];

            nClippedTriangles = TriangleClipAgainstPlane({0.0f, 0.0f, 0.1f}, {0.0f, 0.0f, 1.0f},
                                            triProjected, clipped[0], clipped[1]);
            
            for (int n = 0; n < nClippedTriangles; n++){
                    // Project the vec3d with the projection matrix
                triProjected.p[0] = matMultiplyVector(camMain.matCamProj, clipped[n].p[0]);
                triProjected.p[1] = matMultiplyVector(camMain.matCamProj, clipped[n].p[1]);
                triProjected.p[2] = matMultiplyVector(camMain.matCamProj, clipped[n].p[2]);

                triTranslate.p[0] = triProjected.p[0] + vecTranslate;
                triTranslate.p[1] = triProjected.p[1] + vecTranslate;
                triTranslate.p[2] = triProjected.p[2] + vecTranslate;

                // Scale into view
                triScale.p[0] = triTranslate.p[0] * vecScale;
                triScale.p[1] = triTranslate.p[1] * vecScale;
                triScale.p[2] = triTranslate.p[2] * vecScale;
                triScale.colour = triProjected.colour;
                
                vecTrianglesToRaster.push_back(triScale);
            }
        }
    }
}

void VEngine::draw_mesh(vCamera& camMain, vMesh& mesh, bool outline){
    std::vector<triangle> vecTrianglesToRaster;

    // Camera Scale and offset
    vec3d vecScale(0.5f * (float)camMain.nScreenW, 0.5f * (float)camMain.nScreenH, 1.0f);
    // Camera offset 
    vec3d vecOffset(camMain.nScreenOX, camMain.nScreenOY, 0.0f);
    // Translate coordinate (0,0,0) to centre of camera. 1 to the edges.
    vec3d vecTranslate(1.0f + camMain.fOffsetX , 1.0f + camMain.fOffsetY , 0.0f);

    // Call draw mesh to populate vecTrianglesToRaster
    project_mesh(camMain, vecScale, vecOffset, vecTranslate, mesh, vecTrianglesToRaster, outline);
    draw_to_frame(camMain, vecTrianglesToRaster, outline);
}

// Draw a list of mesh [RE]
void VEngine::draw_scene(vCamera& camMain, std::vector<vMesh>& sceMesh, bool outline){
    std::vector<triangle> vecTrianglesToRaster;

    // Camera Scale and offset
    vec3d vecScale(0.5f * (float)camMain.nScreenW, 0.5f * (float)camMain.nScreenH, 1.0f);
    // Camera offset 
    vec3d vecOffset(camMain.nScreenOX, camMain.nScreenOY, 0.0f);
    // Translate coordinate (0,0,0) to centre of camera. 1 to the edges.
    vec3d vecTranslate(1.0f + camMain.fOffsetX , 1.0f + camMain.fOffsetY , 0.0f);

    for (vMesh& mh : sceMesh){
        project_mesh(camMain, vecScale, vecOffset, vecTranslate, mh, vecTrianglesToRaster, outline);
    }

    draw_to_frame(camMain, vecTrianglesToRaster, outline);
}

void VEngine::draw_scene(vCamera& camMain, std::vector<vMesh*>& sceMesh, bool outline){
    std::vector<triangle> vecTrianglesToRaster;

    // Camera Scale and offset
    vec3d vecScale(0.5f * (float)camMain.nScreenW, 0.5f * (float)camMain.nScreenH, 1.0f);
    // Camera offset 
    vec3d vecOffset(camMain.nScreenOX, camMain.nScreenOY, 0.0f);
    // Translate coordinate (0,0,0) to centre of camera. 1 to the edges.
    vec3d vecTranslate(1.0f + camMain.fOffsetX , 1.0f + camMain.fOffsetY , 0.0f);

    for (vMesh* mh : sceMesh){
        project_mesh(camMain, vecScale, vecOffset, vecTranslate, (*mh), vecTrianglesToRaster, outline);
    }

    draw_to_frame(camMain, vecTrianglesToRaster, outline);
}

vec3d VEngine::project_point(vec3d vPoint, vCamera& cam){
    vec2d res;

    // Project point to camera view
    vPoint = matMultiplyVector(cam.matCamView, vPoint);
    // Project with the camera world matrix
    vPoint = matMultiplyVector(cam.matCamProj, vPoint);
    // Translate (0,0,0) to the centre of frame
    vec3d vecTranslate(1.0f + cam.fOffsetX , 1.0f + cam.fOffsetY , 0.0f);
    vPoint = vPoint + vecTranslate;
     // Scale into the camera's view
    vec3d vecScale(0.5f * (float)cam.nScreenW, 0.5f * (float)cam.nScreenH, 1.0f);
    vPoint = vPoint * vecScale;

    return vPoint;
}

// Drawing a triangle
void VEngine::draw_triangle(triangle& tri){
    // draw the three lines
    
    #ifdef PSVITA
    unsigned int tColour = RGBA8((int)tri.colour.x, (int)tri.colour.y, (int)tri.colour.z, 0xFF);
    vita2d_draw_line(tri.p[0].x, tri.p[0].y,
					 tri.p[1].x, tri.p[1].y, tColour);
    
    vita2d_draw_line(tri.p[0].x, tri.p[0].y,
					 tri.p[2].x, tri.p[2].y, tColour);
    
    vita2d_draw_line(tri.p[2].x, tri.p[2].y,
					 tri.p[1].x, tri.p[1].y, tColour);
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

void VEngine::fill_triangle(triangle& tri, vec3d& colour){

    int R(colour.x), G(colour.y), B(colour.z);
    #ifdef OPENCV
     // Can use the default (faster) but no depth control...
    std::vector<cv::Point> pts = {  cv::Point(tri.p[0].x, tri.p[0].y),
                                    cv::Point(tri.p[1].x, tri.p[1].y),
                                    cv::Point(tri.p[2].x, tri.p[2].y)};
    cv::fillPoly(canvas, pts, cv::Scalar(B, G, R));
    #endif

    #ifdef PSVITA
    unsigned int tColour = RGBA8(R, G, B, 0xFF);
    
    vita2d_color_vertex *vertices = (vita2d_color_vertex *)vita2d_pool_memalign(
			3 * sizeof(vita2d_color_vertex),
			sizeof(vita2d_color_vertex));

    for (int i = 0; i < 3; i++){
        vertices[i].x = tri.p[i].x;
        vertices[i].y = tri.p[i].y;
        vertices[i].z = 0.5f;//tri.p[i].z;
        vertices[i].color = tColour;
    }

    // Draw the array/ (Nice that Vita2D already takes depth into account)
    vita2d_draw_array(SCE_GXM_PRIMITIVE_TRIANGLE_STRIP, vertices, 3);
    #endif
    
    /**
    // Test drawing line
    int ax = tri.p[0].x;
    int ay = tri.p[0].y;
    int bx = tri.p[1].x;
    int by = tri.p[1].y;
    int cx = tri.p[2].x;
    int cy = tri.p[2].y;

    float dbcx, dbcy;
    float dbax, dbay;
    float dacx, dacy;

    dbcx = cx - bx;
    dbcy = cy - by;

    dacx = cx - ax;
    dacy = cy - ay;

    int bcLength = sqrtf(dbcx*dbcx + dbcy*dbcy);
    int acLength = sqrtf(dbax*dbax + dbay*dbay);
    int abLength;
    int skip = 1;
    int skiq = 1;


    dbcx /= bcLength;
    dbcy /= bcLength;
    dacx /= bcLength;
    dacy /= bcLength;


    float nbx(bx), nby(by), nax(ax), nay(ay);
    float nnbx, nnby;
    // draw line from b - > c
    for (int i = 0; i < bcLength; i+= skip){
        //canvas.at<cv::Vec3b>(cv::Point(nbx, nby)) = cv::Vec3b(255, 0, 0);
        //canvas.at<cv::Vec3b>(cv::Point(nax, nay)) = cv::Vec3b(0, 255, 0);
        nbx += dbcx * skip; nby += dbcy * skip;
        nax += dacx * skip; nay += dacy * skip;
        dbax = nax - nbx; dbay = nay - nby;
        abLength = sqrtf(dbax*dbax + dbay*dbay);

        dbax /= abLength;
        dbay /= abLength;

        nnbx = nbx; nnby = nby;
        for (int j = 0; j < abLength; j+= skiq){
            #ifdef OPENCV
                canvas.at<cv::Vec3b>(cv::Point(nnbx, nnby)) = tColour;
            #endif
            #ifdef PSVITA
                vita2d_draw_pixel(nnbx, nnby, tColour);
            #endif
            nnbx += dbax * skiq; nnby += dbay * skiq;
        }
    } **/ 
}