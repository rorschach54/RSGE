#ifndef __RSGE_PHYSICS_COLLISION_SPHERE_H_
#define __RSGE_PHYSICS_COLLISION_SPHERE_H_ 1

/** \file src/include/rsge/physics/collision/sphere.h
 * \brief Collision sphere for Physics
 * \author Spaceboy Ross
 */

#include <rsge/physics/collision/shape.h>
#include <linmath.h>

/**
 * \fn rsge_error_e rsge_physics_sphere_create(rsge_physics_collision_shape_t* shape,int radius)
 * \brief Creates a sphere collision shape.
 * \param[out] shape The pointer to store the collision shape.
 * \param[in] radius The radius of the sphere.
 * \return An error code.
 */
#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_sphere_create(rsge_physics_collision_shape_t* shape,int radius);

#endif
