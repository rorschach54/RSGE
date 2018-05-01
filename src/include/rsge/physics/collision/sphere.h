#ifndef __RSGE_PHYSICS_COLLISION_SPHERE_H_
#define __RSGE_PHYSICS_COLLISION_SPHERE_H_ 1

#include <rsge/physics/collision/shape.h>
#include <linmath.h>

/**
 * \fn rsge_error_e rsge_physics_sphere_create(rsge_physics_collision_shape_t* shape,int radius)
 * \brief Creates a sphere collision shape.
 * \param shape The pointer to store the collision shape.
 * \param radius The radius of the sphere.
 * \return An error code.
 */
#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_sphere_create(rsge_physics_collision_shape_t* shape,int radius);

#endif