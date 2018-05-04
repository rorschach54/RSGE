#include <rsge/gfx/uniforms/point-light.h>
#include <math.h>
#include <string.h>

rsge_error_e rsge_unif_point_light_create(rsge_unif_point_light_t* unif,rsge_point_light_t* pointLight) {
    memset(unif,0,sizeof(rsge_unif_point_light_t));
    unif->pos[0] = pointLight->obj.pos[0];
    unif->pos[1] = pointLight->obj.pos[1];
    unif->pos[2] = pointLight->obj.pos[2];
    unif->pos[3] = 1.0f;
    
    unif->intensity[0] = pointLight->intensity.red;
    unif->intensity[1] = pointLight->intensity.green;
    unif->intensity[2] = pointLight->intensity.blue;
    unif->intensity[3] = pointLight->intensity.alpha;
    
    unif->clipPlanes[0] = 0.1f;
    unif->clipPlanes[1] = 20.0f;
    
    mat4x4_perspective(unif->projection,M_PI/2.0f,M_PI/2.0f,unif->clipPlanes[0],unif->clipPlanes[1]);
    return RSGE_ERROR_NONE;
}