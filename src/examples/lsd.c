#include <rsge/gfx/gl.h>
#include <rsge/gfx/surface.h>
#include <rsge/game.h>
#include <rsge/settings.h>
#include <math.h>
#include <stdlib.h>

rsge_surface_t surf;

rsge_error_e rsge_game_init() {
	int width;
	int height;
	rsge_error_e err = rsge_settings_getint("gfx.res.width",&width);
	if(err != RSGE_ERROR_NONE) return err;
	err = rsge_settings_getint("gfx.res.height",&height);
	if(err != RSGE_ERROR_NONE) return err;
	err = rsge_surface_create(&surf,width,height,4,0);
	if(err != RSGE_ERROR_NONE) return err;
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_game_uninit() {
	rsge_error_e err = rsge_surface_destroy(&surf);
	if(err != RSGE_ERROR_NONE) return err;
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_game_update(double time,int fps) {
	surf.pos[0] = -1.0f;
	surf.pos[1] = -1.0f;
	surf.pos[2] = 0.0f;
	rsge_error_e err = rsge_surface_render(&surf,1.0f,1.0);
	for(size_t y = 0;y < surf.height;y++) {
		for(size_t x = 0;x < surf.width;x++) {
			size_t off = surf.bpp*(x+y*surf.width);
			for(size_t i = 0;i < surf.bpp;i++) surf.buffer[off+i] = (rand() % 255);
		}
	}
	int width;
	int height;
	err = rsge_settings_getint("gfx.res.width",&width);
	if(err != RSGE_ERROR_NONE) return err;
	err = rsge_settings_getint("gfx.res.height",&height);
	if(err != RSGE_ERROR_NONE) return err;
	err = rsge_surface_resize(&surf,width,height,4);
	if(err != RSGE_ERROR_NONE) return err;
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_game_getinfo(rsge_game_t* gameinfo) {
	gameinfo->name = "RSGE Example - LSD";
	gameinfo->id = "rsge.example.lsd";
	return RSGE_ERROR_NONE;
}