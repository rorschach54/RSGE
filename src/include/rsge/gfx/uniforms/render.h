#ifndef __RSGE_GFX_UNIFORMS_RENDER_H_
#define __RSGE_GFX_UNIFORMS_RENDER_H_ 1

/** \file src/include/rsge/gfx/uniforms/render.h
 * \brief Render uniform
 * \author Spaceboy Ross
 */

#include <rsge/error.h>
#include <linmath.h>

/** \struct rsge_unif_render_t src/include/rsge/gfx/uniforms/render.h rsge/gfx/uniforms/render.h
 * \brief Render uniform
 */
typedef struct {
    /**
     * \brief Viewport resolution
     */
    vec2 viewportRes;
} rsge_unif_render_t;

/**
 * \fn rsge_error_e rsge_unif_render_create(rsge_unif_render_t* unif,vec2 viewportRes)
 * \brief Creates a new uniform render.
 * \param[out] unif The pointer to store the uniform render.
 * \param[in] viewportRes The viewport resolution.
 * \return An error code.
 */
rsge_error_e rsge_unif_render_create(rsge_unif_render_t* unif,vec2 viewportRes);

#endif