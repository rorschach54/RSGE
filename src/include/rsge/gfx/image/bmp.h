#ifndef __RSGE_GFX_IMAGE_BMP_H_
#define __RSGE_GFX_IMAGE_BMP_H_ 1

/** \file src/include/rsge/gfx/image/bmp.h
 * \brief Bitmap image
 * \author Spaceboy Ross
 */

#include <rsge/gfx/image.h>

#ifndef DOXYGEN_SHOULD_SKIP_THIS
rsge_error_e rsge_image_bmp_fromFile(rsge_obj_texture_t* texture,char* path);
#endif
#endif
