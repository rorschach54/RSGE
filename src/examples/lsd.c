#include <rsge/gfx/gl.h>
#include <rsge/game.h>
#include <math.h>
#include <stdlib.h>

rsge_error_e rsge_game_init() {
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_game_uninit() {
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_game_update(double time) {
	glClearColor((rand() % 255)*0.01f,(rand() % 255)*0.01f,(rand() % 255)*0.01f,(rand() % 255)*0.01f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_game_getinfo(rsge_game_t* gameinfo) {
	gameinfo->name = "RSGE Example - LSD";
	gameinfo->id = "rsge.example.lsd";
	return RSGE_ERROR_NONE;
}
