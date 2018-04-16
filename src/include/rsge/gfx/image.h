#ifndef __RSGE_GFX_IMAGE_H_
#define __RSGE_GFX_IMAGE_H_ 1

#include <rsge/gfx/surface.h>
#include <rsge/assets.h>

/**
 * \fn rsge_error_e rsge_image_fromFile(rsge_surface_t* surface,rsge_asset_t* asset)
 * \brief Loads an image onto a surface from an asset file.
 * \param surface The pointer to the surface.
 * \param asset The asset file to use.
 */
rsge_error_e rsge_image_fromFile(rsge_surface_t* surface,rsge_asset_t* asset);

#endif
