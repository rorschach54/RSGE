#include <rsge/gfx/gl.h>
#include <rsge/ui/surface.h>
#include <rsge/ui/widget.h>
#include <rsge/game.h>
#include <rsge/ui.h>
#include <math.h>

rsge_ui_surface_t uisurf;

rsge_error_e rsge_game_init() {
	rsge_error_e err = rsge_ui_surface_create(&uisurf,640,480);
	if(err != RSGE_ERROR_NONE) return err;
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_game_uninit() {
	rsge_ui_surface_destroy(&uisurf);
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_game_update(double time,int fps) {
	rsge_error_e err = rsge_ui_surface_update(&uisurf);
	if(err != RSGE_ERROR_NONE) return err;
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_game_getinfo(rsge_game_t* gameinfo) {
	gameinfo->name = "RSGE Example - UI";
	gameinfo->id = "rsge.example.ui";
	return RSGE_ERROR_NONE;
}