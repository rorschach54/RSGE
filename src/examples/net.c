#include <rsge/gfx/gl.h>
#include <rsge/net/http.h>
#include <rsge/assets.h>
#include <rsge/game.h>
#include <math.h>

rsge_error_e rsge_game_init() {
	rsge_error_e err;
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_game_uninit() {
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_game_update(double time) {
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_game_getinfo(rsge_game_t* gameinfo) {
	gameinfo->name = "RSGE Example - Networking";
	gameinfo->id = "rsge.example.net";
	return RSGE_ERROR_NONE;
}