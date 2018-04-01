#ifndef __RSGE_GFX_IMAGE_H_
#define __RSGE_GFX_IMAGE_H_ 1

#include <rsge/gfx/surface.h>

/**
 * \fn rsge_error_e rsge_image_fromFile(rsge_surface_t* surface,char* path)
 * \brief Loads an image onto a surface from a file.
 * \param surface The pointer to the surface.
 * \param path The path to the image.
 */
rsge_error_e rsge_image_fromFile(rsge_surface_t* surface,char* path);

#endif
