/**
#include "../../VEngine/VEngine.h"

class MVEngine : public VEngine{

	private: 
		vCamera camMain = vCamera(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0);
		std::vector<vMesh> scene;
		vMesh meshCube;
	
	void onCreate() override{
		// Change world colour
		world_colour = vec3d(25, 25, 25);

		#ifdef OPENCV
		meshCube.LoadMaterialFile("../res/cube.mtl");
		meshCube.LoadFromObjectFile("../res/cube.obj");
		#endif

		#ifdef PSVITA
		meshCube.LoadMaterialFile("app0:/res/cube.mtl");
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
		scene[0].ApplyRotation(matRot, scene[0].getVecLocation());
		// Draw the scene
		draw_scene(camMain, scene);
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
**/