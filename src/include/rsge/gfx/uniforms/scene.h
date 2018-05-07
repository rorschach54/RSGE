#ifndef __RSGE_GFX_UNIFORMS_SCENE_H_
#define __RSGE_GFX_UNIFORMS_SCENE_H_ 1

/** \file src/include/rsge/gfx/uniforms/scene.h
 * \brief Scene uniform
 * \author Spaceboy Ross
 */

#include <rsge/error.h>
#include <linmath.h>

/** \struct rsge_unif_scene_t src/include/rsge/gfx/uniforms/scene.h rsge/gfx/uniforms/scene.h
 * \brief Scene uniform
 */
typedef struct {
    /**
     * \brief The ambient light intensity.
     */
    vec4 ambientLightIntensity;
} rsge_unif_scene_t;

/**
 * \fn rsge_error_e rsge_unif_scene_create(rsge_unif_scene_t* unif,vec4 ambientLightIntensity)
 * \brief Creates a new uniform scene.
 * \param[out] unif The pointer to store the uniform scene.
 * \param[in] ambientLightIntensity The ambient light intensity.
 * \return An error code.
 */
rsge_error_e rsge_unif_scene_create(rsge_unif_scene_t* unif,vec4 ambientLightIntensity);

#endif