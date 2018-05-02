#ifndef __RSGE_PHYSICS_WORLD_H_
#define __RSGE_PHYSICS_WORLD_H_ 1

/** \file src/include/rsge/physics/world.h
 * \brief Physics world.
 * \author Spaceboy Ross
 */

#include <rsge/physics/rigid-body.h>
#include <rsge/error.h>
#ifdef __cplusplus
#include <btBulletDynamicsCommon.h>
#endif
#include <linmath.h>

/**
 * \struct rsge_physics_world_t src/include/rsge/physics/world.h rsge/physics/world.h
 * \brief Physics world.
 */
typedef struct {
#ifdef __cplusplus
    /**
     * \brief The bullet physics world class.
     */
    btDiscreteDynamicsWorld* btWorld;
#endif
} rsge_physics_world_t;

/**
 * \fn rsge_error_e rsge_physics_world_create(rsge_physics_world_t* world)
 * \brief Creates a new world.
 * \param[out] world The pointer to store the world.
 * \return An error code.
 */
#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_world_create(rsge_physics_world_t* world);

/**
 * \fn rsge_error_e rsge_physics_world_destroy(rsge_physics_world_t* world)
 * \brief Destroyes the world.
 * \param[out] world The pointer to store the world.
 * \return An error code.
 */
#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_world_destroy(rsge_physics_world_t* world);

/**
 * \fn rsge_error_e rsge_physics_world_setgravity(rsge_physics_world_t* world,vec3 gravity)
 * \brief Sets the world's gravity.
 * \param[out] world The pointer to store the world.
 * \param[in] gravity The amount of gravity to give.
 * \return An error code.
 */
#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_world_setgravity(rsge_physics_world_t* world,vec3 gravity);

/**
 * \fn rsge_error_e rsge_physics_world_addrb(rsge_physics_world_t* world,rsge_physics_rigid_body_t* rb)
 * \brief Adds a rigid body to the world.
 * \param[out] world The pointer to store the world.
 * \param[in] rb The rigid body to add.
 * \return An error code.
 */
#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_world_addrb(rsge_physics_world_t* world,rsge_physics_rigid_body_t* rb);

/**
 * \fn rsge_error_e rsge_physics_world_stepSimulation(rsge_physics_world_t* world,float timeStep,int maxSubSteps)
 * \brief stepSimulation proceeds the simulation over 'timeStep', units in preferably in seconds.
 * \param[out] world The pointer to store the world.
 * \param[in] timeStep The time step unit.
 * \param[in] maxSubSteps The max sub steps.
 * \return An error code.
 */
#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_world_stepSimulation(rsge_physics_world_t* world,float timeStep,int maxSubSteps);

#endif