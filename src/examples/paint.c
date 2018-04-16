#include <rsge/gfx/gl.h>
#include <rsge/gfx/surface.h>
#include <rsge/assets/rsge_assets.h>
#include <rsge/assets.h>
#include <rsge/game.h>

rsge_surface_t surf;

rsge_error_e rsge_game_init() {
	rsge_error_e err = rsge_surface_create(&surf,16,16,3,0);
	if(err != RSGE_ERROR_NONE) return err;
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_game_uninit() {
	rsge_error_e err = rsge_surface_destroy(&surf);
	if(err != RSGE_ERROR_NONE) return err;
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_game_update(double time) {
	surf.pos[0] = 0.0f;
	surf.pos[1] = 0.0f;
	surf.pos[2] = 0.0f;
	rsge_error_e err = rsge_surface_render(&surf,1.0f,1.0f);
	if(err != RSGE_ERROR_NONE) return err;
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_game_getinfo(rsge_game_t* gameinfo) {
	gameinfo->name = "RSGE Example - Paint";
	gameinfo->id = "rsge.example.paint";
	return RSGE_ERROR_NONE;
}
