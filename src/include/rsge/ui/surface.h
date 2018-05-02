#ifndef __RSGE_UI_SURFACE_H_
#define __RSGE_UI_SURFACE_H_ 1

/** \file src/include/rsge/ui/surface.h
 * \brief UI surface stuff.
 * \author Spaceboy Ross
 */

#include <rsge/gfx/surface.h>
#include <list.h>

/** \struct rsge_ui_surface_t src/include/rsge/ui/surface.h rsge/ui/surface.h
 * \brief UI surface.
 */
typedef struct {
    /**
     * \brief The graphics surface.
     */
    rsge_surface_t surface;
    
    /**
     * \brief The widgets in the UI.
     */
    list_t* widgets;
} rsge_ui_surface_t;

/**
 * \fn rsge_error_e rsge_ui_surface_create(rsge_ui_surface_t* ui,size_t width,size_t height)
 * \brief Creates a new UI surface.
 * \param[out] ui The pointer to the UI variable.
 * \param[in] width The width of the UI.
 * \param[in] height The height of the UI.
 * \return An error code.
 */
rsge_error_e rsge_ui_surface_create(rsge_ui_surface_t* ui,size_t width,size_t height);

/**
 * \fn rsge_error_e rsge_ui_surface_destroy(rsge_ui_surface_t* ui)
 * \brief Destroyes the UI surface.
 * \param[out] ui The pointer to the UI variable.
 * \return An error code.
 */
rsge_error_e rsge_ui_surface_destroy(rsge_ui_surface_t* ui);

/**
 * \fn rsge_error_e rsge_ui_surface_update(rsge_ui_surface_t* ui)
 * \brief Updates and renders the UI surface.
 * \param[out] ui The pointer to the UI variable.
 * \return An error code.
 */
rsge_error_e rsge_ui_surface_update(rsge_ui_surface_t* ui);

#endif
