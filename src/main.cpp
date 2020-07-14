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

		meshCube.setColour(255, 0, 0);
		scene.push_back(meshCube);
		meshCube.setColour(0, 255, 0);
		scene.push_back(meshCube);
		meshCube.setColour(0, 0, 255);
		scene.push_back(meshCube);

		#ifdef OPENCV
		meshCube.LoadFromObjectFile("../res/Nier.obj");
		scene.push_back(meshCube);
		#endif
		vec3d vecTrans( 0.0f, 0.0f, 5.0f );
		scene[0].ApplyTranslation(vecTrans);
		scene[1].ApplyTranslation(vecTrans);
		scene[2].ApplyTranslation(vecTrans);
		vecTrans = vec3d(0.0f, 0.0f, -10.0f);
		camMain.ApplyTranslation(vecTrans);
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
				//mat4x4 matYawRot = matMakeRotationY( 8.0f * fElapsedTime );
				mat4x4 matYawRot = matMakeRotationAxis(camMain.getVecLocation(), camMain.getVecVertical(), 2.0f * fElapsedTime);
				matRot = matYawRot;
			}
			if (keypress == 69 || keypress == 101){ 
				mat4x4 matYawRot = matMakeRotationAxis(camMain.getVecLocation(), camMain.getVecVertical(),-2.0f * fElapsedTime);
				matRot = matYawRot;
			}
			// IK for Pitch, x axis rotation
			if (keypress == 73 || keypress == 105){ 
				mat4x4 matPitchRot = matMakeRotationAxis(camMain.getVecLocation(), camMain.getVecHorizontal(), 2.0f * fElapsedTime);
				matRot = matMultiplyMatrix(matPitchRot, matRot);
			}
			
			if (keypress == 107 || keypress == 75) {
				mat4x4 matPitchRot = matMakeRotationAxis(camMain.getVecLocation(), camMain.getVecHorizontal(),-2.0f * fElapsedTime);
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

		// Draw some text
		char buff1[36];
		sprintf(buff1, "CAM: %6.3f, %6.3f, %6.3f", camMain.getVecLocation().x, camMain.getVecLocation().y, camMain.getVecLocation().z);
		char buff2[36];
		sprintf(buff2, "FOW: %6.3f, %6.3f, %6.3f", camMain.getVecForward().x, camMain.getVecForward().y, camMain.getVecForward().z);
		char buff3[36];
		sprintf(buff3, "VET: %6.3f, %6.3f, %6.3f", camMain.getVecVertical().x, camMain.getVecVertical().y, camMain.getVecVertical().z);
		char buff4[36];
		sprintf(buff4, "HOR: %6.3f, %6.3f, %6.3f", camMain.getVecHorizontal().x, camMain.getVecHorizontal().y, camMain.getVecHorizontal().z);
		#ifdef OPENCV
		cv::putText(canvas, buff1, cv::Point(10, 40),
                        cv::FONT_HERSHEY_DUPLEX,
                        0.5, CV_RGB(255, 255, 255), 1);
		cv::putText(canvas, buff2, cv::Point(10, 60),
                        cv::FONT_HERSHEY_DUPLEX,
                        0.5, CV_RGB(255, 255, 255), 1);
		cv::putText(canvas, buff3, cv::Point(10, 80),
                        cv::FONT_HERSHEY_DUPLEX,
                        0.5, CV_RGB(255, 255, 255), 1);
		cv::putText(canvas, buff4, cv::Point(10,100),
                        cv::FONT_HERSHEY_DUPLEX,
                        0.5, CV_RGB(255, 255, 255), 1);
		#endif
		#ifdef PSVITA
		vita2d_font_draw_text(font, 20, 40, WHITE, 11, buff1);
		vita2d_font_draw_text(font, 20, 60, WHITE, 11, buff2);
		vita2d_font_draw_text(font, 20, 80, WHITE, 11, buff3);
		vita2d_font_draw_text(font, 20,100, WHITE, 11, buff4);
		#endif
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