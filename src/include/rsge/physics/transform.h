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

#endif