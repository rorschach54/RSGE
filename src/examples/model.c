#include <rsge/gfx/colors.h>
#include <rsge/gfx/gl.h>
#include <rsge/gfx/model.h>
#include <rsge/gfx/shape.h>
#include <rsge/assets/rsge_assets.h>
#include <rsge/assets.h>
#include <rsge/game.h>
#include <math.h>

rsge_shape_t model;

rsge_error_e rsge_game_init() {
	rsge_error_e err;
	rsge_asset_t modelFile;
	err = rsge_asset_find(&rsge_assets,&modelFile,"models/simple.xml");
	if(err != RSGE_ERROR_NONE) return err;

	err = rsge_model_fromFile(&model,&modelFile);
	if(err != RSGE_ERROR_NONE) return err;

	model.pos[0] = 0.0f;
	model.pos[1] = 0.0f;
	model.pos[2] = 0.0f;
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_game_uninit() {
	rsge_shape_destroy(&model);
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_game_update(double time) {
	rsge_shape_render(&model);
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_game_getinfo(rsge_game_t* gameinfo) {
	gameinfo->name = "RSGE Example - Model";
	gameinfo->id = "rsge.example.model";
	return RSGE_ERROR_NONE;
}