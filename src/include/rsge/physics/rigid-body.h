#ifndef __RSGE_PHYSICS_RIGID_BODY_H_
#define __RSGE_PHYSICS_RIGID_BODY_H_ 1

/** \file src/include/rsge/physics/rigid-body.h
 * \brief Physics rigid body.
 * \author Spaceboy Ross
 */

#include <rsge/physics/collision/shape.h>
#include <rsge/physics/motion-state.h>
#include <rsge/error.h>
#ifdef __cplusplus
#include <btBulletDynamicsCommon.h>
#endif
#include <linmath.h>

/** \struct rsge_physics_rigid_body_t src/include/rsge/physics/rigid-body.h rsge/physics/rigid-body.h
 * \brief Rigid body
 */
typedef struct {
#ifdef __cplusplus
    /**
     * \brief The bullet physics rigid body.
     */
    btRigidBody* btRB;
#else
    /**
     * \brief The bullet physics rigid body.
     */
    void* btRB;
#endif
} rsge_physics_rigid_body_t;

/**
 * \fn rsge_error_e rsge_physics_rigid_body_create(rsge_physics_rigid_body_t* rb,rsge_physics_collision_shape_t* shape,vec3 localInertia,float mass)
 * \brief Creates a new rigid body.
 * \param[out] rb The pointer to store the rigid body.
 * \param[in] shape The collision shape to use.
 * \param[in] localInertia The local intertia to use.
 * \param[in] mass The mass to give the rigid body.
 * \return An error code.
 */
#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_rigid_body_create(rsge_physics_rigid_body_t* rb,rsge_physics_collision_shape_t* shape,vec3 localInertia,float mass);

/**
 * \fn rsge_error_e rsge_physics_rigid_body_destroy(rsge_physics_rigid_body_t* rb)
 * \brief Destroyes the rigid body.
 * \param[out] rb The pointer to store the rigid body.
 * \return An error code.
 */
#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_rigid_body_destroy(rsge_physics_rigid_body_t* rb);

/**
 * \fn rsge_error_e rsge_physics_rigid_body_setuserpointer(rsge_physics_rigid_body_t* rb,void* userpointer)
 * \brief Sets the user pointer data of the rigid body.
 * \param[out] rb The pointer to store the rigid body.
 * \param[in] userpointer The user pointer data.
 * \return An error code.
 */
#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_rigid_body_setuserpointer(rsge_physics_rigid_body_t* rb,void* userpointer);

/**
 * \fn rsge_error_e rsge_physics_rigid_body_getuserpointer(rsge_physics_rigid_body_t* rb,void** userpointer)
 * \brief Gets the user pointer data of the rigid body.
 * \param[out] rb The pointer to store the rigid body.
 * \param[out] userpointer The pointer to store the user pointer.
 * \return An error code.
 */
#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_rigid_body_getuserpointer(rsge_physics_rigid_body_t* rb,void** userpointer);

/**
 * \fn rsge_error_e rsge_physics_rigid_body_translate(rsge_physics_rigid_body_t* rb,vec3 pos)
 * \brief Translates the rigid body.
 * \param[out] rb The pointer to store the rigid body.
 * \param[in] pos The position to use.
 * \return An error code.
 */
#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_rigid_body_translate(rsge_physics_rigid_body_t* rb,vec3 pos);

/**
 * \fn rsge_error_e rsge_physics_rigid_body_getmotionstate(rsge_physics_rigid_body_t* rb,rsge_physics_motion_state_t* motionState)
 * \brief Gets the rigid body's motion state.
 * \param[out] rb The pointer to store the rigid body.
 * \param[out] motionState The pointer to store the motion state.
 * \return An error code.
 */
#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_rigid_body_getmotionstate(rsge_physics_rigid_body_t* rb,rsge_physics_motion_state_t* motionState);

/**
 * \fn rsge_error_e rsge_physics_rigid_body_setmotionstate(rsge_physics_rigid_body_t* rb,rsge_physics_motion_state_t* motionState)
 * \brief Sets the rigid body's motion state.
 * \param[out] rb The pointer to store the rigid body.
 * \param[out] motionState The pointer to store the motion state.
 * \return An error code.
 */
#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_rigid_body_setmotionstate(rsge_physics_rigid_body_t* rb,rsge_physics_motion_state_t* motionState);

#endif