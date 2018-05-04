#ifndef __RSGE_GFX_UNIFORMS_MATERIAL_H_
#define __RSGE_GFX_UNIFORMS_MATERIAL_H_ 1

/** \file src/include/rsge/uniforms/material.h
 * \brief Material uniform
 * \author Spaceboy Ross
 */

#include <rsge/error.h>
#include <linmath.h>

/** \struct rsge_unif_material_t src/include/rsge/uniforms/material.h rsge/uniforms/material.h
 * \brief Material uniform
 */
typedef struct {
    /**
     * \brief The resolution of the bump map.
     */
    vec2 bumpMapResolution;
    
    /**
     * \brief The specular exponent value.
     */
    float specularExponent;
} rsge_unif_material_t;

/**
 * \fn rsge_error_e rsge_unif_material_create(rsge_unif_material_t* unif)
 * \brief Creates a new uniform material.
 * \param[out] unif The pointer to store the uniform material.
 * \return An error code.
 */
rsge_error_e rsge_unif_material_create(rsge_unif_material_t* unif);

#endif