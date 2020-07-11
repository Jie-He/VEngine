#include "VEngine.h"

class mVEngine : public VEngine{

	private:
		mesh meshCube;

		float fTheta = 0;

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

	}

	void update (float fElapsedTime) override{
		//vita2d_font_draw_text(font, 128, 55, WHITE, 11, "updateFunction");
		fTheta += 1.0f * fElapsedTime;
		mat4x4 matRotZ, matRotX, matRotXZ;
		matRotX = matMakeRotationX(0.5 * fTheta);
		matRotZ = matMakeRotationZ(      fTheta);
		matRotXZ= matMultiplyMatrix(matRotZ, matRotX);

		mesh cube = meshCube;
		// rotate each face in mesh
		for (int i = 0; i < cube.tris.size(); i++){
			cube.tris[i].p[0] = matMultiplyVector(matRotXZ, cube.tris[i].p[0]);
			cube.tris[i].p[1] = matMultiplyVector(matRotXZ, cube.tris[i].p[1]);
			cube.tris[i].p[2] = matMultiplyVector(matRotXZ, cube.tris[i].p[2]);

			// shift each face back in z direction
			cube.tris[i].p[0].z += 6.0f;
			cube.tris[i].p[1].z += 6.0f;
			cube.tris[i].p[2].z += 6.0f;
		}

		draw_mesh(cube);
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
