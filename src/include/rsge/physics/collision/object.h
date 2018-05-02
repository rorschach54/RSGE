#ifndef __RSGE_PHYSICS_COLLISION_OBJECT_H_
#define __RSGE_PHYSICS_COLLISION_OBJECT_H_ 1

/** \file src/include/rsge/physics/collision/object.h
 * \brief Collision object for Physics
 * \author Spaceboy Ross
 */

#include <rsge/physics/collision/shape.h>
#include <rsge/error.h>
#ifdef __cplusplus
#include <btBulletDynamicsCommon.h>
#endif
#include <linmath.h>

/** \struct rsge_physics_collision_object_t src/include/rsge/physics/collision/object.h rsge/physics/collision/object.h
 * \brief A collision object.
 */
typedef struct {
#ifdef __cplusplus
    /**
     * \brief The bullet physics collision object.
     */
    btCollisionObject* btObject;
#endif
} rsge_physics_collision_object_t;

/**
 * \fn rsge_error_e rsge_physics_collision_object_getcollisionshape(rsge_physics_collision_object_t* object,rsge_physics_collision_shape_t* shape)
 * \brief Gets the collision shape from the collision object.
 * \param[out] object The collision object to use.
 * \param[out] shape The pointer to store the collision shape.
 * \return An error code.
 */
#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_collision_object_getcollisionshape(rsge_physics_collision_object_t* object,rsge_physics_collision_shape_t* shape);

#endif