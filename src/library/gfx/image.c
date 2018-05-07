#include <rsge/gfx/image/bmp.h>
#include <rsge/gfx/image.h>
#include <rsge/config.h>

#if CONFIG_USE_LIBPNG == 1
#include <rsge/gfx/image/libpng.h>
#endif

rsge_error_e rsge_image_fromFile(rsge_obj_texture_t* texture,char* path) {
	rsge_error_e err;

#if CONFIG_USE_LIBPNG == 1
	err = rsge_image_libpng_fromFile(texture,path);
	if(err == RSGE_ERROR_NONE) return err;
#endif

	err = rsge_image_bmp_fromFile(texture,path);
	if(err == RSGE_ERROR_NONE) return err;
	return RSGE_ERROR_INVALID_FILE;
}