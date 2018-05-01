#ifndef __RSGE_PHYSICS_INIT_H_
#define __RSGE_PHYSICS_INIT_H_ 1

#include <rsge/error.h>
#ifdef __cplusplus
#include <btBulletDynamicsCommon.h>

extern btBroadphaseInterface* rsge_physics_bpiface;
extern btDefaultCollisionConfiguration* rsge_physics_collision_cfg;
extern btCollisionDispatcher* rsge_physics_collision_dispatcher;
extern btSequentialImpulseConstraintSolver* rsge_physics_solver;
#endif

/**
 * \fn rsge_error_e rsge_physics_init()
 * \brief Initializes the physics engine. This is done autmatically.
 * \return An error code.
 */
#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_init();

/**
 * \fn rsge_error_e rsge_physics_deinit()
 * \brief Deinitializes the physics engine. This is done autmatically.
 * \return An error code.
 */
#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_deinit();

#endif