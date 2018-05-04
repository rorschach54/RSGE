#include <rsge/gfx/camera.h>
#include <math.h>
#include <string.h>

rsge_camera_t rsge_camera;

float rsge_camera_col[4];

rsge_error_e rsge_camera_getmaincam(rsge_camera_t** cam) {
	*cam = &rsge_camera;
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_camera_create(rsge_camera_t* cam) {
	memset(cam,0,sizeof(rsge_camera_t));
	quat_identity(cam->rot);
	
	cam->fov[0] = M_PI/2.0f;
	cam->fov[1] = M_PI/2.0f;
	cam->clipPlanes[0] = 0.1f;
	cam->clipPlanes[1] = 50.0f;
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_camera_setcolor(int color[4]) {
	rsge_camera_col[0] = color[0]/0.01f;
	rsge_camera_col[1] = color[1]/0.01f;
	rsge_camera_col[2] = color[2]/0.01f;
	rsge_camera_col[3] = color[3]/0.01f;
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_camera_update() {
	glClearColor(rsge_camera_col[0],rsge_camera_col[1],rsge_camera_col[2],rsge_camera_col[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glLoadIdentity();
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_camera_reshape(int width,int height) {
	GLfloat ratio = (GLfloat)height/(GLfloat)width;
	glViewport(0,0,(GLint)width,(GLint)height);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	glMatrixMode(GL_MODELVIEW);
	return RSGE_ERROR_NONE;
}