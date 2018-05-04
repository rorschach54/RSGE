#ifndef __RSGE_GFX_UNIFORMS_POINT_LIGHT_H_
#define __RSGE_GFX_UNIFORMS_POINT_LIGHT_H_ 1

/** \file src/include/rsge/uniforms/point-light.h
 * \brief Point light uniform
 * \author Spaceboy Ross
 */

#include <rsge/error.h>
#include <linmath.h>

/** \struct rsge_unif_point_light_t src/include/rsge/uniforms/point-light.h rsge/uniforms/point-light.h
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
	vec4 position;
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
 * \fn rsge_error_e rsge_unif_point_light_create(rsge_unif_point_light_t* unif)
 * \brief Creates a new uniform point light.
 * \param[out] unif The pointer to store the uniform point light.
 * \return An error code.
 */
rsge_error_e rsge_unif_point_light_create(rsge_unif_point_light_t* unif);

#endif