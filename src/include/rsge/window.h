#ifndef __RSGE_WINDOW_H_
#define __RSGE_WINDOW_H_ 1

/** \file src/include/rsge/window.h
 * \brief Window stuff.
 * \author Spaceboy Ross
 */

#include <rsge/gfx/objects/texture.h>
#include <rsge/gfx/surface.h>
#include <rsge/error.h>

/**
 * \fn rsge_error_e rsge_window_seticon_fromSurfaces(rsge_surface_t** surfaces,int surfaceCount)
 * \brief Sets the window icon to an array of surfaces.
 * \deprecated Use the from textures version of this function.
 * \param[in] surfaces The surfaces to use.
 * \param[in] surfaceCount The number of surfaces.
 * \return An error code.
 */
rsge_error_e rsge_window_seticon_fromSurfaces(rsge_surface_t** surfaces,int surfaceCount) __attribute__ ((deprecated));

/**
 * \fn rsge_error_e rsge_window_seticon_fromTextures(rsge_obj_texture_t** textures,int textureCount)
 * \brief Sets the window icon to an array of textures.
 * \param[in] textures The textures to use.
 * \param[in] textureCount The number of textures.
 * \return An error code.
 */
rsge_error_e rsge_window_seticon_fromTextures(rsge_obj_texture_t** textures,int textureCount);

/**
 * \fn rsge_error_e rsge_window_seticon_fromFiles(char** paths,int pathCount)
 * \brief Sets the window icon.
 * \param[in] paths The paths to the assets to use.
 * \param[in] pathCount The number of paths.
 * \return An error code.
 */
rsge_error_e rsge_window_seticon_fromFiles(char** paths,int pathCount);

#endif
