#ifndef __RSGE_GFX_SCREENSHOT_H_
#define __RSGE_GFX_SCREENSHOT_H_ 1

#include <rsge/gfx/gl.h>
#include <rsge/gfx/surface.h>

/**
 * \fn rsge_error_e rsge_screenshot_tosurface(rsge_surface_t* surface)
 * \brief Stores a screenshot in a surface.
 * \param surface The surface to store the screenshot.
 * \return An error code.
 */
rsge_error_e rsge_screenshot_tosurface(rsge_surface_t* surface);

#endif