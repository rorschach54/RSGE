#ifndef __RSGE_GFX_IMAGE_LIBPNG_H_
#define __RSGE_GFX_IMAGE_LIBPNG_H_ 1

#include <rsge/gfx/image.h>
#include <rsge/assets.h>
#include <rsge/config.h>

#if CONFIG_USE_LIBPNG == 1
#include <png.h>

rsge_error_e rsge_image_libpng_fromFile(rsge_surface_t* surface,char* path);

#endif
#endif