#ifndef __RSGE_PHYSICS_RAYCASTING_H_
#define __RSGE_PHYSICS_RAYCASTING_H_ 1

/** \file src/include/rsge/physics/raycasting.h
 * \brief Physics raycasting.
 * \author Spaceboy Ross
 */

#include <rsge/physics/collision/object.h>
#include <rsge/physics/world.h>
#include <rsge/error.h>
#ifdef __cplusplus
#include <btBulletDynamicsCommon.h>
#endif
#include <linmath.h>

typedef struct {
#ifdef __cplusplus
    /**
     * \brief Bullet physics raycasting callback.
     */
    btCollisionWorld::ClosestRayResultCallback RayCallback;
#else
    /**
     * \brief Bullet physics raycasting callback.
     */
    void* RayCallback;
#endif

    /**
     * \brief The object collided.
     */
    rsge_physics_collision_object_t collisionObject;
} rsge_physics_raycast_t;

/**
 * \fn rsge_error_e rsge_physics_raycast_createFromMouse(rsge_physics_raycast_t* ray,rsge_physics_world_t* world)
 * \brief Raycasts from the mouse's current position.
 * \param[out] ray The pointer to store the raycast.
 * \param[in] world The physics world to use.
 * \return An error code.
 */
#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_raycast_createFromMouse(rsge_physics_raycast_t* ray,rsge_physics_world_t* world);

/**
 * \fn rsge_error_e rsge_physics_raycast_create(rsge_physics_raycast_t* ray,rsge_physics_world_t* world,vec3 outOrigin,vec3 outDir)
 * \brief Raycasts on a world.
 * \param[out] ray The pointer to store the raycast.
 * \param[in] world The physics world to use.
 * \param[in] outOrigin The out origin.
 * \param[in] outDir The out direction.
 * \return An error code.
 */
#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_raycast_create(rsge_physics_raycast_t* ray,rsge_physics_world_t* world,vec3 outOrigin,vec3 outDir);

/**
 * \fn rsge_error_e rsge_physics_raycast_destroy(rsge_physics_raycast_t* ray)
 * \brief Destroyes the raycast.
 * \param[out] ray The pointer to store the raycast.
 * \return An error code.
 */
#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_raycast_destroy(rsge_physics_raycast_t* ray);

/**
 * \fn rsge_error_e rsge_physics_raycast_hashit(rsge_physics_raycast_t* ray,int* hit)
 * \brief Determins if the raycast has a hit.
 * \param[out] ray The pointer to store the raycast.
 * \param[out] hit The pointer to store the hit.
 * \return An error code.
 */
#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_raycast_hashit(rsge_physics_raycast_t* ray,int* hit);

#endif