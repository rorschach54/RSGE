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