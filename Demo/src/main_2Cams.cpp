#include "../../VEngine/VEngine.h"

/**
class mVEngine : public VEngine{

	private:
		//mesh meshCube;
		vCamera camMain = vCamera(360, 360, 80, 92, 0.1f, 100.0f, 90.0f);
		vCamera camMai2 = vCamera(360, 360, 520,92);
		std::vector<vMesh> scene;

	void onCreate() override{
		vMesh meshCube;
		vMesh plane;

		// Loading the basic cube
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
		vec3d vtranslate(-2.0f, 0.0f, -5.0f);
		camMain.ApplyTranslation(vtranslate);

		vtranslate = vec3d(2.0f, 0.0f, -5.0f);
		camMai2.ApplyTranslation(vtranslate);
	}

	void update (float fElapsedTime) override{
		// rotate each face in mesh
		float rad = ((3.14159f / 4.0f ) * fElapsedTime);
		mat4x4 matRotX = matMakeRotationX( rad );
		mat4x4 matRotZ = matMakeRotationZ( rad/2 );
		mat4x4 matRot  = matMultiplyMatrix(matRotZ, matRotX);

		scene[0].ApplyRotation(matRot, scene[0].getVecLocation());

		// Some basic camera control
		vec3d vecTrans;
		matRot = matIdentity();
		// If opencv mode read keypress char
		#ifdef OPENCV
			// WASD for changing view vertically and horizontally
			if (keypress == 87 || keypress == 119) vecTrans -= (camMain.getVecVertical()  * 8.0f) * fElapsedTime;
			if (keypress == 83 || keypress == 115) vecTrans += (camMain.getVecVertical()  * 8.0f) * fElapsedTime;
			if (keypress == 65 || keypress ==  97) vecTrans -= (camMain.getVecHorizontal() * 8.0f) * fElapsedTime;
			if (keypress == 68 || keypress == 100) vecTrans += (camMain.getVecHorizontal() * 8.0f) * fElapsedTime;
			// ZX for forward and backward
			if (keypress == 90 || keypress == 122) vecTrans += (camMain.getVecForward() * 8.0f) * fElapsedTime;
			if (keypress == 88 || keypress == 120) vecTrans -= (camMain.getVecForward() * 8.0f) * fElapsedTime;
			
			// QE for Vertical axis rotation (look left and right)
			if (keypress == 81 || keypress == 113) {
				mat4x4 matYawRot = matMakeRotationAxis(camMain.getVecVertical(), 2.0f * fElapsedTime);
				matRot = matYawRot;
			}
			if (keypress == 69 || keypress == 101){ 
				mat4x4 matYawRot = matMakeRotationAxis(camMain.getVecVertical(), -2.0f * fElapsedTime);
				matRot = matYawRot;
			}
			// IK for Pitch, Horizontal axis rotation (up and down)
			if (keypress == 73 || keypress == 105){ 
				mat4x4 matPitchRot = matMakeRotationAxis(camMain.getVecHorizontal(), 2.0f * fElapsedTime);
				matRot = matMultiplyMatrix(matPitchRot, matRot);
			}
			
			if (keypress == 107 || keypress == 75) {
				mat4x4 matPitchRot = matMakeRotationAxis(camMain.getVecHorizontal(), -2.0f * fElapsedTime);
				matRot = matMultiplyMatrix(matPitchRot, matRot);
			}
		#endif

		#ifdef PSVITA
			// DPAD for vertical and horizontal view change
			if (ctrl.buttons & SCE_CTRL_UP   )	vecTrans -= (camMain.getVecVertical()   * 8.0f) * fElapsedTime;
			if (ctrl.buttons & SCE_CTRL_DOWN )  vecTrans += (camMain.getVecVertical()   * 8.0f) * fElapsedTime;
			if (ctrl.buttons & SCE_CTRL_LEFT )  vecTrans -= (camMain.getVecHorizontal() * 8.0f) * fElapsedTime;
			if (ctrl.buttons & SCE_CTRL_RIGHT)  vecTrans += (camMain.getVecHorizontal() * 8.0f) * fElapsedTime;

			// L and R trigger for back and forward movement respectively
			if (ctrl.buttons & SCE_CTRL_RTRIGGER) vecTrans += (camMain.getVecForward() * 8.0f) * fElapsedTime;
			if (ctrl.buttons & SCE_CTRL_LTRIGGER) vecTrans -= (camMain.getVecForward() * 8.0f) * fElapsedTime;
			
			// [TODO] Need to rotate by vecUP and vecRight
			// Right stick for camera rotation control
			// Introducing 50 as deadzone for the stick (bcs my vita's sticks are broke af)
			// Left right rotation
			if (abs(ctrl.rx - 128) > 50){
				mat4x4 matYawRot = matMakeRotationAxis(camMain.getVecVertical(), (128 -ctrl.rx) / 50.0f * fElapsedTime );
				matRot = matMultiplyMatrix(matYawRot, matRot);
			}
			
			// Up down rotation (inverted)
			if (abs(ctrl.ry - 128) > 50){
				mat4x4 matPitchRot = matMakeRotationAxis(camMain.getVecHorizontal(), (ctrl.ry - 128) / 50.0f * fElapsedTime );
				matRot = matMultiplyMatrix(matPitchRot, matRot);
			}
		#endif
		
		camMain.ApplyTranslation(vecTrans);
		camMain.ApplyRotation(matRot, camMain.getVecLocation());

		// Update cam 2
		vecTrans = vecNegative(vecTrans);
		camMai2.ApplyTranslation(vecTrans);
		camMai2.ApplyRotation(matRot, camMai2.getVecLocation());

		draw_scene(camMain, scene);
		draw_scene(camMai2, scene);
	}
};

int main(int argc, char *argv[]) {

	#ifdef OPENCV
		std::cout << "OPENCV MODE" << std::endl;
	#endif

	mVEngine mve;
	mve.start();

	#ifdef PSVITA
		// Call exit
		sceKernelExitProcess(0);
	#endif

    return 0;
}
**/