#ifndef __RSGE_WINDOW_H_
#define __RSGE_WINDOW_H_ 1

/** \file src/include/rsge/window.h
 * \brief Window stuff.
 * \author Spaceboy Ross
 */

#include <rsge/gfx/surface.h>
#include <rsge/error.h>

/**
 * \fn rsge_error_e rsge_window_seticon_fromSurfaces(rsge_surface_t** surfaces,int surfaceCount)
 * \brief Sets the window icon to an array of surfaces.
 * \param[in] surfaces The surfaces to use.
 * \param[in] surfaceCount The number of surfaces.
 * \return An error code.
 */
rsge_error_e rsge_window_seticon_fromSurfaces(rsge_surface_t** surfaces,int surfaceCount);

/**
 * \fn rsge_error_e rsge_window_seticon_fromFiles(char** paths,int pathCount)
 * \brief Sets the window icon.
 * \param[in] paths The paths to the assets to use.
 * \param[in] pathCount The number of paths.
 * \return An error code.
 */
rsge_error_e rsge_window_seticon_fromFiles(char** paths,int pathCount);

#endif
