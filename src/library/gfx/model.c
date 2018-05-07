#include <rsge/gfx/model/obj.h>
#include <rsge/gfx/model.h>
#include <rsge/config.h>
#include <string.h>

rsge_error_e rsge_model_fromFile(list_t** objects,char* path) {
	rsge_error_e err;

	err = rsge_model_obj_fromFile(objects,path);
	if(err != RSGE_ERROR_NONE) return err;
	return RSGE_ERROR_NONE;
}