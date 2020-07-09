#include <psp2/kernel/processmgr.h>

#include <sstream>
#include <vector>
#include <cstdio>

#include "VEngine.h"

int main(int argc, char *argv[]) {

	// Later move to VE
	vita2d_init();
	vita2d_set_clear_color(BLACK);
	font = vita2d_load_font_mem(basicfont, basicfont_size);

	_VENGINE_::vec2d point = {20.0f, 30.0f};

	

	while(1){
		
	}

	vita2d_fini();
	vita2d_free_font(font); 
	sceKernelExitProcess(0);
    return 0;
}
