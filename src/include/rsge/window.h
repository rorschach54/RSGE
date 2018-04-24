#ifndef __RSGE_WINDOW_H_
#define __RSGE_WINDOW_H_ 1

#include <rsge/gfx/surface.h>
#include <rsge/error.h>

/**
 * \fn rsge_error_e rsge_window_seticon_fromSurfaces(rsge_surface_t** surfaces,int surfaceCount)
 * \brief Sets the window icon to an array of surfaces.
 * \param surfaces The surfaces to use.
 * \param surfaceCount The number of surfaces.
 * \return An error code.
 */
rsge_error_e rsge_window_seticon_fromSurfaces(rsge_surface_t** surfaces,int surfaceCount);

/**
 * \fn rsge_error_e rsge_window_seticon_fromFiles(char** paths,int pathCount)
 * \brief Sets the window icon.
 * \param paths The paths to the assets to use.
 * \param pathCount The number of paths.
 * \return An error code.
 */
rsge_error_e rsge_window_seticon_fromFiles(char** paths,int pathCount);

#endif