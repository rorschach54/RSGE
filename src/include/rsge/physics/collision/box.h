#ifndef __RSGE_PHYSICS_COLLISION_BOX_H_
#define __RSGE_PHYSICS_COLLISION_BOX_H_ 1

/** \file src/include/rsge/physics/collision/box.h
 * \brief Collision box for Physics
 * \author Spaceboy Ross
 */

#include <rsge/physics/collision/shape.h>
#include <linmath.h>

/**
 * \fn rsge_error_e rsge_physics_box_create(rsge_physics_collision_shape_t* shape,vec3 pos)
 * \brief Creates a box collision shape.
 * \param[out] shape The pointer to store the collision shape.
 * \param[in] pos The position of the shape.
 * \return An error code.
 */
#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_box_create(rsge_physics_collision_shape_t* shape,vec3 pos);

#endif
