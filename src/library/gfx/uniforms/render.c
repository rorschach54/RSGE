#include <rsge/gfx/uniforms/render.h>
#include <string.h>

rsge_error_e rsge_unif_render_create(rsge_unif_render_t* unif,vec2 viewportRes) {
    memset(unif,0,sizeof(rsge_unif_render_t));
    unif->viewportRes[0] = viewportRes[0];
    unif->viewportRes[1] = viewportRes[1];
    return RSGE_ERROR_NONE;
}