#ifndef __RSGE_GFX_UNIFORMS_OBJECT_H_
#define __RSGE_GFX_UNIFORMS_OBJECT_H_ 1

/** \file src/include/rsge/gfx/gfx/uniforms/object.h
 * \brief Object uniform
 * \author Spaceboy Ross
 */

#include <rsge/gfx/object.h>
#include <rsge/error.h>
#include <linmath.h>

/** \struct rsge_unif_obj_t src/include/rsge/gfx/uniforms/object.h rsge/gfx/uniforms/object.h
 * \brief Object uniform
 */
typedef struct {
    /**
     * \brief Transform.
     */
    mat4x4 transform;
} rsge_unif_obj_t;

/**
 * \fn rsge_error_e rsge_unif_obj_create(rsge_unif_obj_t* unif,rsge_object_t* obj)
 * \brief Creates a new uniform object.
 * \param[out] unif The pointer to store the uniform object.
 * \param[in] obj The object to use.
 * \return An error code.
 */
rsge_error_e rsge_unif_obj_create(rsge_unif_obj_t* unif,rsge_object_t* obj);

#endif