#ifndef __RSGE_GFX_IMAGE_LIBPNG_H_
#define __RSGE_GFX_IMAGE_LIBPNG_H_ 1

/** \file src/include/rsge/gfx/image/libpng.h
 * \brief libpng image
 * \author Spaceboy Ross
 */

#include <rsge/gfx/image.h>
#include <rsge/assets.h>
#include <rsge/config.h>

#if CONFIG_USE_LIBPNG == 1
#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include <png.h>

rsge_error_e rsge_image_libpng_fromFile(rsge_obj_texture_t* texture,char* path);

#endif
#endif
#endif
