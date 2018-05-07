#ifndef __RSGE_GFX_UNIFORMS_POINT_LIGHT_H_
#define __RSGE_GFX_UNIFORMS_POINT_LIGHT_H_ 1

/** \file src/include/rsge/gfx/uniforms/point-light.h
 * \brief Point light uniform
 * \author Spaceboy Ross
 */

#include <rsge/gfx/lighting.h>
#include <rsge/error.h>
#include <linmath.h>

/** \struct rsge_unif_point_light_t src/include/rsge/gfx/uniforms/point-light.h rsge/gfx/uniforms/point-light.h
 * \brief Point light uniform
 */
typedef struct {
    /**
     * \brief The transform
     */
    mat4x4 transform[6];
    /**
     * \brief The projection
     */
	mat4x4 projection;
	/**
	 * \brief The position
	 */
	vec4 pos;
	/**
	 * \brief The intensity
	 */
	vec4 intensity;
	/**
	 * \brief Clip planes.
	 */
	vec2 clipPlanes;
} rsge_unif_point_light_t;

/**
 * \fn rsge_error_e rsge_unif_point_light_create(rsge_unif_point_light_t* unif,rsge_point_light_t* pointLight)
 * \brief Creates a new uniform point light.
 * \param[out] unif The pointer to store the uniform point light.
 * \param[in] pointLight The point light to use.
 * \return An error code.
 */
rsge_error_e rsge_unif_point_light_create(rsge_unif_point_light_t* unif,rsge_point_light_t* pointLight);

#endif