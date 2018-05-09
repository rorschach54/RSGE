#include <rsge/gfx/model/obj.h>
#include <rsge/gfx/model.h>
#include <rsge/config.h>
#include <string.h>

#if CONFIG_USE_ASSIMP == 1
#include <rsge/gfx/model/assimp.h>
#endif

rsge_error_e rsge_model_fromFile(list_t** objects,char* path) {
	rsge_error_e err;
	
#if CONFIG_USE_ASSIMP == 1
	err = rsge_model_assimp_fromFile(objects,path);
	if(err != RSGE_ERROR_NONE) return err;
#endif

	err = rsge_model_obj_fromFile(objects,path);
	if(err != RSGE_ERROR_NONE) return err;
	return RSGE_ERROR_NONE;
}