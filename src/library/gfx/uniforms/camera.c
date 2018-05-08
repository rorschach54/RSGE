#include <rsge/gfx/uniforms/camera.h>
#include <string.h>

rsge_error_e rsge_unif_cam_create(rsge_unif_cam_t* unif,rsge_camera_t* cam) {
    if(cam == NULL) {
        rsge_camera_t* tmp_cam;
        rsge_error_e err = rsge_camera_getmaincam(&tmp_cam);
        if(err != RSGE_ERROR_NONE) return err;
        return rsge_unif_cam_create(unif,tmp_cam);
    }
    memset(unif,0,sizeof(rsge_unif_cam_t));
    
	mat4x4_perspective(unif->projectionMatrix,cam->fov[0],cam->fov[1],cam->clipPlanes[0],cam->clipPlanes[1]);
	mat4x4_invert(unif->inverseProjectionMatrix,unif->projectionMatrix);
	
	mat4x4_from_quat(unif->viewMatrix,cam->rot);
	unif->viewMatrix[3][0] = cam->pos[0];
	unif->viewMatrix[3][1] = cam->pos[1];
	unif->viewMatrix[3][2] = cam->pos[2];
	mat4x4_invert(unif->inverseViewMatrix,unif->viewMatrix);
	
	unif->fov[0] = cam->fov[0];
	unif->fov[1] = cam->fov[1];
	
	unif->zNear = cam->clipPlanes[0];
	unif->zFar = cam->clipPlanes[1];
    return RSGE_ERROR_NONE;
}