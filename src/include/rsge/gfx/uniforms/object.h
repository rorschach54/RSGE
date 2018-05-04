#ifndef __RSGE_GFX_UNIFORMS_OBJECT_H_
#define __RSGE_GFX_UNIFORMS_OBJECT_H_ 1

/** \file src/include/rsge/uniforms/object.h
 * \brief Object uniform
 * \author Spaceboy Ross
 */

#include <rsge/error.h>
#include <linmath.h>

/** \struct rsge_unif_obj_t src/include/rsge/uniforms/object.h rsge/uniforms/object.h
 * \brief Object uniform
 */
typedef struct {
    /**
     * \brief Transform.
     */
    mat4x4 transform;
} rsge_unif_obj_t;

/**
 * \fn rsge_error_e rsge_unif_obj_create(rsge_unif_obj_t* unif)
 * \brief Creates a new uniform object.
 * \param[out] unif The pointer to store the uniform object.
 * \return An error code.
 */
rsge_error_e rsge_unif_obj_create(rsge_unif_obj_t* unif);

#endif