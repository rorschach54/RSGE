#ifndef __RSGE_GFX_IMAGE_H_
#define __RSGE_GFX_IMAGE_H_ 1

/** \file src/include/rsge/gfx/image.h
 * \brief Image stuff
 * \author Spaceboy Ross
 */

#include <rsge/gfx/surface.h>
#include <rsge/assets.h>

/**
 * \fn rsge_error_e rsge_image_fromFile(rsge_surface_t* surface,char* path)
 * \brief Loads an image onto a surface from an asset file.
 * \param[out] surface The pointer to the surface.
 * \param[in] path The path to the asset.
 * \return An error code.
 */
rsge_error_e rsge_image_fromFile(rsge_surface_t* surface,char* path);

#endif
