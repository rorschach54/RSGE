#ifndef __RSGE_PHYSICS_TRANSFORM_H_
#define __RSGE_PHYSICS_TRANSFORM_H_ 1

/** \file src/include/rsge/physics/transform.h
 * \brief Physics transform.
 * \author Spaceboy Ross
 */

#include <rsge/error.h>
#ifdef __cplusplus
#include <btBulletDynamicsCommon.h>
#endif
#include <linmath.h>

/**
 * \struct rsge_physics_transform_t src/include/rsge/physics/transform.h rsge/physics/transform.h
 * \brief Physics transform.
 */
typedef struct {
#ifdef __cplusplus
    /**
     * \brief Bullet physics transform.
     */
    btTransform btTF;
#endif
} rsge_physics_transform_t;

/**
 * \fn rsge_error_e rsge_physics_transform_getorigin(rsge_physics_transform_t* transform,vec3* pos)
 * \brief Gets the origin of the tranform.
 * \param[out] transform The transform to use.
 * \param[out] pos The pointer to the position.
 * \return An error code.
 */
#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_transform_getorigin(rsge_physics_transform_t* transform,vec3* pos);

#endif