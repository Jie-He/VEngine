#include "../../VEngine/VEngine.h"

class MVEngine : public VEngine{

	private: 
		vCamera camMain = vCamera(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 1.0f, 2.0f);
		std::vector<vMesh> scene;
		vMesh meshCube;
	
	void onCreate() override{
		// Change world colour
		world_colour = vec3d(25, 25, 25);

		#ifdef OPENCV
		meshCube.LoadFromObjectFile("../res/cube.obj");
		#endif

		#ifdef PSVITA
		meshCube.LoadFromObjectFile("app0:/res/cube.obj");
		#endif

		meshCube.setLocation(vec3d(0.0f, 0.0f, 5.0f));
		scene.push_back(meshCube);

		// Shift the camera
		vec3d vtranslate(0.0f, 0.0f, -5.0f);
		camMain.ApplyTranslation(vtranslate);
	}

	void update(float fElapsedTime) override{
		float rad = ((3.14159f / 4.0f ) * fElapsedTime);
		mat4x4 matRotX = matMakeRotationX( rad   );
		mat4x4 matRotZ = matMakeRotationZ( rad/2 );
		mat4x4 matRot  = matMultiplyMatrix(matRotZ, matRotX);

		// Rotate the cube
		//scene[0].ApplyRotation(matRot, scene[0].getVecLocation());
		meshCube.ApplyRotation(matRot, meshCube.getVecLocation());
		// Draw the scene
		//draw_scene(camMain, scene);
		draw_mesh(camMain, meshCube);

		float rx, ry;
		rx = (rand() % 2) - 1;
		ry = (rand() % 2) - 1;

		camMain.ApplyTranslation(vec3d(rx * fElapsedTime, ry * fElapsedTime, fElapsedTime));

		vec3d cube_centre = project_point(meshCube.getVecLocation(), camMain);
		#ifdef OPENCV
		cv::circle(canvas, cv::Point(cube_centre.x, cube_centre.y), 5, cv::Scalar(0,0,255));
		char buff[20];
		sprintf(buff, "Depth Z: %6.3f", cube_centre.z);
		cv::putText(canvas, buff, cv::Point(cube_centre.x + 5, cube_centre.y - 5),
					 cv::FONT_HERSHEY_DUPLEX, 0.5, CV_RGB(255,255,255), 1);
		#endif
	}

};

int main(int argc, char *argv[]) {

	MVEngine mve;
	mve.start();

	#ifdef PSVITA
	sceKernelExitProcess(0);
	#endif 
	return 0;
}
