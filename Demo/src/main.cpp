#include "../../VEngine/VEngine.h"

class MVEngine : public VEngine{

	private: 
		vCamera camMain = vCamera(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0);
		std::vector<vMesh> scene;
	
	void onCreate() override{
		// Load the demo cube
		vMesh meshCube;

		#ifdef OPENCV
		meshCube.LoadMaterialFile("../res/cube.mtl");
		meshCube.LoadFromObjectFile("../res/cube.obj");
		#endif

		#ifdef PSVITA
		meshCube.LoadMaterialFile("app0:/res/cube.mtl");
		meshCube.LoadFromObjectFile("app0:/res/cube.obj");
		#endif

		vec3d vt(0.0f, 0.0f, 10.0f);
		meshCube.ApplyTranslation(vt);

		scene.push_back(meshCube);

		// Shift the camera
		vec3d vtranslate(0.0f, 0.0f, -10.0f);
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