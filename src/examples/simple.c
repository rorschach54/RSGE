#include <rsge/gfx/gl.h>
#include <rsge/game.h>

rsge_error_e rsge_game_init() {
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_game_uninit() {
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_game_update(double time) {
	glClearColor(0.0f,0.0f,0.0f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_game_getinfo(rsge_game_t* gameinfo) {
	gameinfo->name = "RSGE Example";
	return RSGE_ERROR_NONE;
}
