#include "VEngine.h"

class mVEngine : public VEngine{

	private:
		//mesh meshCube;
		std::vector<vMesh> scene;

	void onCreate() override{
		
		vMesh meshCube;

		// Loading the basic cube
		#ifdef OPENCV
	 	meshCube.LoadFromObjectFile("../res/Cube.obj");
		#endif

		#ifdef PSVITA
		meshCube.LoadFromObjectFile("app0:/res/Cube.obj");
		#endif

		scene.push_back(meshCube);
		scene.push_back(meshCube);
		scene.push_back(meshCube);

		vec3d vecTrans( 0.0f, 0.0f, 5.0f );
		scene[0].ApplyTranslation(vecTrans);
		scene[1].ApplyTranslation(vecTrans);
		scene[2].ApplyTranslation(vecTrans);
	}

	void update (float fElapsedTime) override{
		//vita2d_font_draw_text(font, 128, 55, WHITE, 11, "updateFunction");
		// rotate each face in mesh
		float rad = ((3.14159f / 4.0f ) * fElapsedTime);
		mat4x4 matRotX = matMakeRotationX( rad );
		mat4x4 matRotY = matMakeRotationY( rad );
		mat4x4 matRotZ = matMakeRotationZ( rad );
		//mat4x4 matRot  = matMultiplyMatrix(matRotY, matRotX);
		// 	   matRot  = matMultiplyMatrix(matRotZ, matRot );
		
		//meshCube.ApplyRotation(matRot, meshCube.origin);
		vec3d vecZero;
		scene[0].ApplyRotation(matRotX, vecZero);
		scene[1].ApplyRotation(matRotY, vecZero);
		scene[2].ApplyRotation(matRotZ, vecZero);

		// Some basic camera control
		vec3d vecTrans;
		mat4x4 matRot = matIdentity();
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
			
			// QE for Y axis rotation
			if (keypress == 81 || keypress == 113) {
				mat4x4 matYawRot = matMakeRotationY( 8.0f * fElapsedTime );
				matRot = matMultiplyMatrix(matYawRot, matRot);
			}
			if (keypress == 69 || keypress == 101){ 
				mat4x4 matYawRot = matMakeRotationY(-8.0f * fElapsedTime );
				matRot = matMultiplyMatrix(matYawRot, matRot);
			}
			// IK for Pitch, x axis rotation
			if (keypress == 73 || keypress == 105){ 
				mat4x4 matPitchRot = matMakeRotationX( 8.0f * fElapsedTime );
				matRot = matMultiplyMatrix(matPitchRot, matRot);
			}
			
			if (keypress == 107 || keypress == 75) {
				mat4x4 matPitchRot = matMakeRotationX(-8.0f * fElapsedTime );
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
				mat4x4 matYawRot = matMakeRotationY( (128 -ctrl.rx) / 50.0f * fElapsedTime );
				matRot = matMultiplyMatrix(matYawRot, matRot);
			}
			
			// Up down rotation (inverted)
			if (abs(ctrl.ry - 128) > 50){
				mat4x4 matPitchRot = matMakeRotationX( (ctrl.ry - 128) / 50.0f * fElapsedTime );
				matRot = matMultiplyMatrix(matPitchRot, matRot);
			}
		#endif

		camMain.ApplyTranslation(vecTrans);
		camMain.ApplyRotation(matRot, camMain.getVecLocation());

		draw_scene(scene);
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