#ifndef __RSGE_PHYSICS_COLLISION_SHAPE_H_
#define __RSGE_PHYSICS_COLLISION_SHAPE_H_ 1

#include <rsge/error.h>
#ifdef __cplusplus
#include <btBulletDynamicsCommon.h>
#endif
#include <linmath.h>

typedef struct {
#ifdef __cplusplus
    /**
     * \brief The bullet physics collision shape.
     */
    btCollisionShape* btShape;
#endif
} rsge_physics_collision_shape_t;

/**
 * \fn rsge_error_e rsge_physics_shape_destroy(rsge_physics_collision_shape_t* shape)
 * \brief Destroyes the collision shape.
 * \param shape The pointer that stores the shape.
 * \return An error code.
 */
#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_shape_destroy(rsge_physics_collision_shape_t* shape);

#endif