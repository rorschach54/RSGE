#include <rsge/gfx/objects/texture.h>
#include <rsge/gfx/uniforms/material.h>
#include <string.h>

rsge_error_e rsge_unif_material_create(rsge_unif_material_t* unif,rsge_material_t* mat) {
    memset(unif,0,sizeof(rsge_unif_material_t));
    rsge_obj_texture_t* bumpMap;
    rsge_error_e err = rsge_obj_texture_getbyid(mat->bumpMapID,&bumpMap);
    if(err != RSGE_ERROR_NONE) return err;
    unif->bumpMapResolution[0] = bumpMap->width;
    unif->bumpMapResolution[1] = bumpMap->height;
    unif->specularExponent = mat->specularExponent;
    return RSGE_ERROR_NONE;
}