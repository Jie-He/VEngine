#include "VEngine.h"

class mVEngine : public VEngine{
	void update (float fElapsedTime) override{
		vita2d_font_draw_text(font, 20, 100, WHITE, 11, "Hello from UPDATE!");
	}
};

int main(int argc, char *argv[]) {

	mVEngine mve;
	mve.start();

	sceKernelExitProcess(0);
    return 0;
}
