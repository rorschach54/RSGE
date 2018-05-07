#ifndef __RSGE_GFX_SCREENSHOT_H_
#define __RSGE_GFX_SCREENSHOT_H_ 1

/** \file src/include/rsge/gfx/screenshot.h
 * \brief Screenshot stuff
 * \author Spaceboy Ross
 */

#include <rsge/gfx/objects/texture.h>
#include <rsge/gfx/gl.h>
#include <rsge/gfx/surface.h>

/**
 * \fn rsge_error_e rsge_screenshot_tosurface(rsge_surface_t* surface)
 * \brief Stores a screenshot in a surface.
 * \deprecated Use the totexture version.
 * \param[out] surface The surface to store the screenshot.
 * \return An error code.
 */
rsge_error_e rsge_screenshot_tosurface(rsge_surface_t* surface);

/**
 * \fn rsge_error_e rsge_screenshot_totexture(rsge_obj_texture_t* texture)
 * \brief Stores a screenshot in a texture.
 * \deprecated Use the totexture version.
 * \param[out] texture The texture to store the screenshot.
 * \return An error code.
 */
rsge_error_e rsge_screenshot_totexture(rsge_obj_texture_t* texture);

#endif
