#ifndef __RSGE_GFX_IMAGE_H_
#define __RSGE_GFX_IMAGE_H_ 1

/** \file src/include/rsge/gfx/image.h
 * \brief Image stuff
 * \author Spaceboy Ross
 */

#include <rsge/gfx/objects/texture.h>
#include <rsge/assets.h>

/**
 * \fn rsge_error_e rsge_image_fromFile(rsge_obj_texture_t* texture,char* path)
 * \brief Loads an image onto a surface from an asset file.
 * \param[out] texture The pointer to the texture.
 * \param[in] path The path to the asset.
 * \return An error code.
 */
rsge_error_e rsge_image_fromFile(rsge_obj_texture_t* texture,char* path);

#endif
