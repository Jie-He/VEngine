#include "VEngine.h"

class mVEngine : public VEngine{

	private:
		mesh meshCube;

	void onCreate() override{
		
		#ifdef OPENCV
		meshCube.LoadFromObjectFile("../res/Nier.obj");
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
		mat4x4 matRotX = matMakeRotationX( 1.0f * fElapsedTime );
		mat4x4 matRotY = matMakeRotationY( 0.5f * fElapsedTime );
		mat4x4 matRotZ = matMakeRotationZ( -1.5f * fElapsedTime );
		mat4x4 matRot  = matMultiplyMatrix(matRotY, matRotX);
			   matRot  = matMultiplyMatrix(matRotZ, matRot );
		
		meshCube.ApplyRotation(matRot, meshCube.origin);

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

// Define the vertex for a cube, could keep it for debugging.
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
