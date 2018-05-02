#ifndef __RSGE_PHYSICS_COLLISION_BOX_H_
#define __RSGE_PHYSICS_COLLISION_BOX_H_ 1

/** \file src/include/rsge/physics/collision/box.h
 * \brief Collision box for Physics
 * \author Spaceboy Ross
 */

#include <rsge/physics/collision/shape.h>
#include <linmath.h>

/**
 * \fn rsge_error_e rsge_physics_box_create(rsge_physics_collision_shape_t* shape,vec3 boxHalfExtents)
 * \brief Creates a box collision shape.
 * \param[out] shape The pointer to store the collision shape.
 * \param[in] boxHalfExtents The half extents of the box.
 * \return An error code.
 */
#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_box_create(rsge_physics_collision_shape_t* shape,vec3 boxHalfExtents);

#endif
