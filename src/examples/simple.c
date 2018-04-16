#include <rsge/gfx/colors.h>
#include <rsge/gfx/font.h>
#include <rsge/gfx/gl.h>
#include <rsge/gfx/shape.h>
#include <rsge/gfx/surface.h>
#include <rsge/assets/rsge_assets.h>
#include <rsge/assets.h>
#include <rsge/game.h>

rsge_shape_t tri;
rsge_font_t font;
rsge_surface_t text;
GLuint objects;
GLubyte objects_lists[2];

rsge_error_e rsge_game_init() {
	rsge_asset_t* fontFile;
	rsge_error_e err;
	err = rsge_asset_find(&rsge_assets,&fontFile,"fonts/tarpino/tarpino.ttf");
	if(err != RSGE_ERROR_NONE) return err;

	err = rsge_font_fromFile(&font,fontFile,4);
	if(err != RSGE_ERROR_NONE) return err;

	font.invert = false;

	err = rsge_font_render(&font,&text,"Hello, world!",RSGE_COLOR_BLUE);
	if(err != RSGE_ERROR_NONE) return err;

	text.flags |= RSGE_SURFACE_FLAG_MULTIDISPLLST;

	text.pos[0] = 0.0f;
	text.pos[1] = 0.0f;
	text.pos[2] = 0.0f;

	err = rsge_shape_create(&tri,GL_TRIANGLES,(rsge_shape_vert_t[3]){
		{ .flags = RSGE_SHAPE_VERT_FLAG_COLOR, .pos = { -0.6f,-0.4f,0.0f }, .color = { 1.0f,0.0f,0.0f,1.0f } },
		{ .flags = RSGE_SHAPE_VERT_FLAG_COLOR, .pos = { 0.6f,-0.4f,0.0f }, .color = { 0.0f,1.0f,0.0f,1.0f } },
		{ .flags = RSGE_SHAPE_VERT_FLAG_COLOR, .pos = { 0.0f,0.6f,0.0f }, .color = { 0.0f,0.0f,1.0f,0.0f } }
	},3,RSGE_SHAPE_FLAG_MULTIDISPLLST);
	if(err != RSGE_ERROR_NONE) return err;

	tri.pos[0] = 0.0f;
	tri.pos[1] = 0.0f;
	tri.pos[2] = 0.0f;

	objects = glGenLists(2);
	glNewList(objects,GL_COMPILE);
	rsge_shape_render(&tri);
	glEndList();

	glNewList(objects+1,GL_COMPILE);
	rsge_surface_render(&text,0.5f,0.5f);
	glEndList();
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_game_uninit() {
	rsge_shape_destroy(&tri);
	rsge_font_destroy(&font);
	rsge_surface_destroy(&text);
	glDeleteLists(2,objects);
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_game_update(double time) {
	objects_lists[0] = 0;
	objects_lists[1] = 1;
	glListBase(objects);

	glCallLists(2,GL_UNSIGNED_BYTE,objects_lists);
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_game_getinfo(rsge_game_t* gameinfo) {
	gameinfo->name = "RSGE Example - Simple";
	gameinfo->id = "rsge.example.simple";
	return RSGE_ERROR_NONE;
}