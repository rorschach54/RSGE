#ifndef __RSGE_PHYSICS_MOTION_STATE_H_
#define __RSGE_PHYSICS_MOTION_STATE_H_ 1

/** \file src/include/rsge/physics/motion-state.h
 * \brief Physics motion state.
 * \author Spaceboy Ross
 */

#include <rsge/physics/transform.h>
#include <rsge/error.h>
#ifdef __cplusplus
#include <btBulletDynamicsCommon.h>
#endif
#include <linmath.h>

/**
 * \struct rsge_physics_motion_state_t src/include/rsge/physics/motion-state.h rsge/physics/motion-state.h
 * \brief Physics motion state.
 */
typedef struct {
#ifdef __cplusplus
    /**
     * \brief Bullet physics motion state.
     */
    btMotionState* btMS;
#else
    /**
     * \brief Bullet physics motion state.
     */
    void* btMS;
#endif
} rsge_physics_motion_state_t;

/**
 * \fn rsge_error_e rsge_physics_motion_state_setworldtransform(rsge_physics_motion_state_t* motionState,rsge_physics_transform_t* transform)
 * \brief Sets the motion state world transformation.
 * \param[out] motionState The pointer to store the motion state.
 * \param[in] transform The transform that is used.
 * \return An error code.
 */
#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_motion_state_setworldtransform(rsge_physics_motion_state_t* motionState,rsge_physics_transform_t* transform);

/**
 * \fn rsge_error_e rsge_physics_motion_state_getworldtransform(rsge_physics_motion_state_t* motionState,rsge_physics_transform_t* transform)
 * \brief Gets the motion state world transformation.
 * \param[out] motionState The pointer to store the motion state.
 * \param[out] transform The pointer to store the transform.
 * \return An error code.
 */
#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_motion_state_getworldtransform(rsge_physics_motion_state_t* motionState,rsge_physics_transform_t* transform);

#endif