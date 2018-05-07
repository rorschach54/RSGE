#include <rsge/gfx/uniforms/object.h>
#include <string.h>

rsge_error_e rsge_unif_obj_create(rsge_unif_obj_t* unif,rsge_object_t* obj) {
    memset(unif,0,sizeof(rsge_unif_obj_t));
    mat4x4 rot;
    mat4x4_from_quat(rot,obj->rot);
    mat4x4_translate(unif->transform,obj->pos[0],obj->pos[1],obj->pos[2]);
    for(int i = 0;i < 4;i++) {
        for(int j = 0;j < 4;j++) {
            unif->transform[i][j] *= rot[i][j];
        }
    }
    return RSGE_ERROR_NONE;
}