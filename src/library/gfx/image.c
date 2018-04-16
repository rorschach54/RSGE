#include <rsge/gfx/image.h>
#include <rsge/config.h>

#if CONFIG_USE_LIBPNG == 1
#include <rsge/gfx/image/libpng.h>
#endif

rsge_error_e rsge_image_fromFile(rsge_surface_t* surface,rsge_asset_t* asset) {
	rsge_error_e err;

#if CONFIG_USE_LIBPNG == 1
	err = rsge_image_libpng_fromFile(surface,asset);
	if(err == RSGE_ERROR_NONE) return err;
#endif
	return RSGE_ERROR_INVALID_FILE;
}
