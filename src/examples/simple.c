#include <rsge/gfx/colors.h>
#include <rsge/gfx/font.h>
#include <rsge/gfx/gl.h>
#include <rsge/gfx/shape.h>
#include <rsge/gfx/surface.h>
#include <rsge/assets/rsge_assets.h>
#include <rsge/assets.h>
#include <rsge/game.h>
#include <math.h>

rsge_shape_t tri;
rsge_font_t font;
rsge_surface_t text;

rsge_error_e rsge_game_init() {
	rsge_error_e err;
	err = rsge_font_fromFile(&font,"rsge@fonts/thintel/thintel.ttf",4);
	if(err != RSGE_ERROR_NONE) return err;

	font.invert = false;

	err = rsge_font_render(&font,&text,"Hello, world!",RSGE_COLOR_BLUE);
	if(err != RSGE_ERROR_NONE) return err;

	text.pos[0] = -1.0f;
	text.pos[1] = 0.7f;
	text.pos[2] = 0.0f;

	err = rsge_shape_create(&tri,GL_TRIANGLES,(rsge_shape_vert_t[3]){
		{ .flags = RSGE_SHAPE_VERT_FLAG_COLOR, .pos = { -0.6f, -0.4f, 0.0f }, .color = { 1.0f, 0.0f, 0.0f, 1.0f } },
		{ .flags = RSGE_SHAPE_VERT_FLAG_COLOR, .pos = { 0.6f, -0.4f, 0.0f }, .color = { 0.0f, 1.0f, 0.0f, 1.0f } },
		{ .flags = RSGE_SHAPE_VERT_FLAG_COLOR, .pos = { 0.0f, 0.6f, 0.0f }, .color = { 0.0f, 0.0f, 1.0f, 1.0f } }
	},3,NULL,0,0);
	if(err != RSGE_ERROR_NONE) return err;

	tri.pos[0] = 0.0f;
	tri.pos[1] = 0.0f;
	tri.pos[2] = 0.0f;
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_game_uninit() {
	rsge_shape_destroy(&tri);
	rsge_font_destroy(&font);
	rsge_surface_destroy(&text);
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_game_update(double time) {
	rsge_shape_render(&tri);
	rsge_surface_render(&text,0.004f,0.004f);
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_game_getinfo(rsge_game_t* gameinfo) {
	gameinfo->name = "RSGE Example - Simple";
	gameinfo->id = "rsge.example.simple";
	return RSGE_ERROR_NONE;
}