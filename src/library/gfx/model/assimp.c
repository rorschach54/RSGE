#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <rsge/gfx/model/assimp.h>
#include <rsge/gfx/objects/texture.h>
#include <rsge/gfx/colors.h>
#include <rsge/gfx/mesh.h>
#include <rsge/assets.h>
#include <log.h>
#include <linmath.h>
#include <string.h>

#ifndef DOXYGEN_SHOULD_SKIP_THIS
typedef struct {
    char* data;
    size_t datasz;
    size_t off;
} rsge_model_assimp_t;

rsge_error_e rsge_model_assimp_fromFile(list_t** objects,char* path) {
    char* data;
    size_t datasz;
    rsge_error_e err = rsge_asset_read(path,&data,&datasz);
    if(err != RSGE_ERROR_NONE) return err;
    
    aiScene* scene = aiImportFileFromMemory(data,datasz,aiProcessPreset_TargetRealtime_MaxQuality,NULL);
    if(!scene) return RSGE_ERROR_ASSIMP;
    return RSGE_ERROR_NONE;
}
#endif