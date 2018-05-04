#include <rsge/gfx/uniforms/scene.h>
#include <string.h>

rsge_error_e rsge_unif_scene_create(rsge_unif_scene_t* unif,vec4 ambientLightIntensity) {
    memset(unif,0,sizeof(rsge_unif_scene_t));
    unif->ambientLightIntensity[0] = ambientLightIntensity[0];
    unif->ambientLightIntensity[1] = ambientLightIntensity[1];
    unif->ambientLightIntensity[2] = ambientLightIntensity[2];
    unif->ambientLightIntensity[3] = ambientLightIntensity[3];
    return RSGE_ERROR_NONE;
}