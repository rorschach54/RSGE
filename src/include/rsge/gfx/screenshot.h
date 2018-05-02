#ifndef __RSGE_GFX_SCREENSHOT_H_
#define __RSGE_GFX_SCREENSHOT_H_ 1

/** \file src/include/rsge/gfx/screenshot.h
 * \brief Screenshot stuff
 * \author Spaceboy Ross
 */

#include <rsge/gfx/gl.h>
#include <rsge/gfx/surface.h>

/**
 * \fn rsge_error_e rsge_screenshot_tosurface(rsge_surface_t* surface)
 * \brief Stores a screenshot in a surface.
 * \param[out] surface The surface to store the screenshot.
 * \return An error code.
 */
rsge_error_e rsge_screenshot_tosurface(rsge_surface_t* surface);

#endif
