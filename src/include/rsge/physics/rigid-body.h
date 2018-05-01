#ifndef __RSGE_PHYSICS_RIGID_BODY_H_
#define __RSGE_PHYSICS_RIGID_BODY_H_ 1

#include <rsge/physics/collision/shape.h>
#include <rsge/error.h>
#ifdef __cplusplus
#include <btBulletDynamicsCommon.h>
#endif
#include <linmath.h>

typedef struct {
#ifdef __cplusplus
    /**
     * \brief The bullet physics rigid body.
     */
    btRigidBody* btRB;
#endif
} rsge_physics_rigid_body_t;

/**
 * \fn rsge_error_e rsge_physics_rigid_body_create(rsge_physics_rigid_body_t* rb,rsge_physics_collision_shape_t* shape,int mass)
 * \brief Creates a new rigid body.
 * \param rb The pointer to store the rigid body.
 * \param shape The collision shape to use.
 * \param mass The mass to give the rigid body.
 * \return An error code.
 */
#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_rigid_body_create(rsge_physics_rigid_body_t* rb,rsge_physics_collision_shape_t* shape,int mass);

/**
 * \fn rsge_error_e rsge_physics_rigid_body_destroy(rsge_physics_rigid_body_t* rb)
 * \brief Destroyes the rigid body.
 * \param rb The pointer to store the rigid body.
 * \return An error code.
 */
#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_rigid_body_destroy(rsge_physics_rigid_body_t* rb);

/**
 * \fn rsge_error_e rsge_physics_rigid_body_setuserpointer(rsge_physics_rigid_body_t* rb,void* userpointer)
 * \brief Sets the user pointer data of the rigid body.
 * \param rb The pointer to store the rigid body.
 * \param userpointer The user pointer data.
 * \return An error code.
 */
#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_rigid_body_setuserpointer(rsge_physics_rigid_body_t* rb,void* userpointer);

/**
 * \fn rsge_error_e rsge_physics_rigid_body_getuserpointer(rsge_physics_rigid_body_t* rb,void** userpointer)
 * \brief Gets the user pointer data of the rigid body.
 * \param rb The pointer to store the rigid body.
 * \param userpointer The pointer to store the user pointer.
 * \return An error code.
 */
#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_rigid_body_getuserpointer(rsge_physics_rigid_body_t* rb,void** userpointer);

#endif