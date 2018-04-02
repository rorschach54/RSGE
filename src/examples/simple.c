#include <rsge/gfx/gl.h>
#include <rsge/gfx/shape.h>
#include <rsge/game.h>

GLuint tri;

rsge_error_e rsge_game_init() {
	glEnableClientState(GL_VERTEX_ARRAY);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDisable(GL_CULL_FACE);
    
	rsge_shape_generate(&tri,GL_TRIANGLES,(rsge_shape_vert_t[3]){
		{ .flags = RSGE_SHAPE_VERT_FLAG_COLOR, .pos = { -0.6f,-0.4f,0.0f }, .color = { 1.0f,0.0f,0.0f,1.0f } },
		{ .flags = RSGE_SHAPE_VERT_FLAG_COLOR, .pos = { 0.6f,-0.4f,0.0f }, .color = { 0.0f,1.0f,0.0f,1.0f } },
		{ .flags = RSGE_SHAPE_VERT_FLAG_COLOR, .pos = { 0.0f,0.6f,0.0f }, .color = { 0.0f,0.0f,1.0f,0.0f } }
	},3);
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_game_uninit() {
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_game_update(double time) {
	glClearColor(0.0f,0.0f,0.0f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glPushMatrix();
	glCallList(tri);
	glPopMatrix();
	glFlush();
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_game_getinfo(rsge_game_t* gameinfo) {
	gameinfo->name = "RSGE Example - Simple";
	gameinfo->id = "rsge.example.simple";
	return RSGE_ERROR_NONE;
}
