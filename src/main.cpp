#include "VEngine.h"

class mVEngine : public VEngine{

	private:
		mesh meshCube;
		mat4x4 matRotZ, matRotX, matRotXZ;

	void onCreate() override{
		// Define the vertex for a cube
		/**
		meshCube.tris = {
							{vec3d(0.0f, 0.0f, 0.0f),	vec3d(0.0f, 1.0f, 0.0f),	vec3d(1.0f, 1.0f, 0.0f)},
							{vec3d(0.0f, 0.0f, 0.0f),	vec3d(1.0f, 1.0f, 0.0f),	vec3d(1.0f, 0.0f, 0.0f)},
							
							{vec3d(1.0f, 0.0f, 0.0f),	vec3d(1.0f, 1.0f, 0.0f),	vec3d(1.0f, 1.0f, 1.0f)},
							{vec3d(1.0f, 0.0f, 0.0f),	vec3d(1.0f, 1.0f, 1.0f),	vec3d(1.0f, 0.0f, 1.0f)},
							
							{vec3d(1.0f, 0.0f, 1.0f),	vec3d(1.0f, 1.0f, 1.0f),	vec3d(0.0f, 1.0f, 1.0f)},
							{vec3d(1.0f, 0.0f, 1.0f),	vec3d(0.0f, 1.0f, 1.0f),	vec3d(0.0f, 0.0f, 1.0f)},
							
							{vec3d(0.0f, 0.0f, 1.0f),	vec3d(0.0f, 1.0f, 1.0f),	vec3d(0.0f, 1.0f, 0.0f)},
							{vec3d(0.0f, 0.0f, 1.0f),	vec3d(0.0f, 1.0f, 0.0f),	vec3d(0.0f, 0.0f, 0.0f)},
							
							{vec3d(0.0f, 1.0f, 0.0f),	vec3d(0.0f, 1.0f, 1.0f),	vec3d(1.0f, 1.0f, 1.0f)},
							{vec3d(0.0f, 1.0f, 0.0f),	vec3d(1.0f, 1.0f, 1.0f),	vec3d(1.0f, 1.0f, 0.0f)},
							
							{vec3d(1.0f, 0.0f, 1.0f),	vec3d(0.0f, 0.0f, 1.0f),	vec3d(0.0f, 0.0f, 0.0f)},
							{vec3d(1.0f, 0.0f, 1.0f),	vec3d(0.0f, 0.0f, 0.0f),	vec3d(1.0f, 0.0f, 0.0f)}
						}; **/
		#ifdef OPENCV
		meshCube.LoadFromObjectFile("../res/Cube.obj");
		std::cout << "Loaded mode!" << std::endl;
 		#endif

		#ifdef PSVITA
		meshCube.LoadFromObjectFile("app0:/res/Cube.obj");
		#endif

		vec3d vecTrans(0.0f, 0.0f, 5.0f);
		meshCube.ApplyTranslation(vecTrans);
	}

	void update (float fElapsedTime) override{
		//vita2d_font_draw_text(font, 128, 55, WHITE, 11, "updateFunction");
		// rotate each face in mesh
		matRotX = matMakeRotationX( 1.0f * fElapsedTime );
		matRotZ = matMakeRotationZ( 2.0f * fElapsedTime );
		matRotXZ= matMultiplyMatrix(matRotZ, matRotX);
		
		vec3d vecPivot(1.0f, 2.0f, 3.0f);
		meshCube.ApplyRotation(matRotXZ, vecPivot);

		draw_mesh(meshCube);
	}
};

int main(int argc, char *argv[]) {

	#ifdef OPENCV
		std::cout << "OPENCV MODE" << std::endl;
	#endif

	mVEngine mve;
	mve.start();

    return 0;
}
