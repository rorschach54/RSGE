#ifndef __RSGE_PHYSICS_COLLISION_BOX_H_
#define __RSGE_PHYSICS_COLLISION_BOX_H_ 1

#include <rsge/physics/collision/shape.h>
#include <linmath.h>

/**
 * \fn rsge_error_e rsge_physics_box_create(rsge_physics_collision_shape_t* shape,vec3 pos)
 * \brief Creates a box collision shape.
 * \param shape The pointer to store the collision shape.
 * \param pos The position of the shape.
 * \return An error code.
 */
#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_box_create(rsge_physics_collision_shape_t* shape,vec3 pos);

#endif