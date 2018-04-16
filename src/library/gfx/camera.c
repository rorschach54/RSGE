#include <rsge/gfx/camera.h>
#include <math.h>

mat4x4 rsge_camera_m;
mat4x4 rsge_camera_p;
mat4x4 rsge_camera_mvp;
vec3 rsge_camera_rot;
vec3 rsge_camera_pos;
float rsge_camera_col[4];

rsge_error_e rsge_camera_setcolor(int color[4]) {
	rsge_camera_col[0] = color[0]/0.01f;
	rsge_camera_col[1] = color[1]/0.01f;
	rsge_camera_col[2] = color[2]/0.01f;
	rsge_camera_col[3] = color[3]/0.01f;
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_camera_setrot(vec2 rot) {
	/* Calculate rotrad position */
	vec2 rotrad;
	rotrad[0] = rot[0]/180*(M_PI*1.0f);
	rotrad[1] = rot[1]/180*(M_PI*1.0f);

	/* Set the camera's position */
	rsge_camera_rot[0] -= sin(rotrad[1]);
	rsge_camera_rot[1] += sin(rotrad[0]);
	rsge_camera_rot[2] += cos(rotrad[1]);
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_camera_setpos(vec3 pos) {
	rsge_camera_pos[0] = pos[0];
	rsge_camera_pos[1] = pos[1];
	rsge_camera_pos[2] = pos[2];
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_camera_getpos(vec3* pos) {
	*pos[0] = rsge_camera_pos[0];
	*pos[1] = rsge_camera_pos[1];
	*pos[2] = rsge_camera_pos[2];
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_camera_update() {
	glClearColor(rsge_camera_col[0],rsge_camera_col[1],rsge_camera_col[2],rsge_camera_col[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glLoadIdentity();
	//glRotatef(rsge_camera_rot[0],1.0f,0.0f,0.0f);
	//glRotatef(rsge_camera_rot[1],0.0f,1.0f,0.0f);
	//glRotatef(rsge_camera_rot[2],0.0f,0.0f,1.0f);
	//glTranslatef(-rsge_camera_pos[0],-rsge_camera_pos[1],-rsge_camera_pos[2]);
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_camera_reshape(int width,int height) {
	GLfloat ratio = (GLfloat)height/(GLfloat)width;
	glViewport(0,0,(GLint)width,(GLint)height);

	/* Update the camera's mvp */
	mat4x4_identity(rsge_camera_m);
	mat4x4_ortho(rsge_camera_p,-ratio,-ratio,-1.0f,1.0f,1.0f,-1.0f);
	mat4x4_mul(rsge_camera_mvp,rsge_camera_p,rsge_camera_m);
	return RSGE_ERROR_NONE;
}