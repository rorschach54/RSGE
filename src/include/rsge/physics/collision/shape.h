#ifndef __RSGE_PHYSICS_COLLISION_SHAPE_H_
#define __RSGE_PHYSICS_COLLISION_SHAPE_H_ 1

/** \file src/include/rsge/physics/collision/shape.h
 * \brief Collision shape for Physics
 * \author Spaceboy Ross
 */

#include <rsge/error.h>
#ifdef __cplusplus
#include <btBulletDynamicsCommon.h>
#endif
#include <linmath.h>

/** \struct rsge_physics_collision_shape_t src/include/rsge/physics/collision/shape.h rsge/physics/collision/shape.h
 * \brief A collision shape.
 */
typedef struct {
#ifdef __cplusplus
    /**
     * \brief The bullet physics collision shape.
     */
    btCollisionShape* btShape;
#else
    /**
     * \brief The bullet physics collision shape.
     */
    void* btShape;
#endif
} rsge_physics_collision_shape_t;

/**
 * \fn rsge_error_e rsge_physics_shape_destroy(rsge_physics_collision_shape_t* shape)
 * \brief Destroyes the collision shape.
 * \param[out] shape The pointer that stores the shape.
 * \return An error code.
 */
#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_shape_destroy(rsge_physics_collision_shape_t* shape);

#endif
