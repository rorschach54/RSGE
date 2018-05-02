#ifndef __RSGE_PHYSICS_TRANSFORM_H_
#define __RSGE_PHYSICS_TRANSFORM_H_ 1

/** \file src/include/rsge/physics/transform.h
 * \brief Physics transform.
 * \author Spaceboy Ross
 */

#include <rsge/error.h>
#ifdef __cplusplus
#include <btBulletDynamicsCommon.h>
#endif
#include <linmath.h>

/**
 * \struct rsge_physics_transform_t src/include/rsge/physics/transform.h rsge/physics/transform.h
 * \brief Physics transform.
 */
typedef struct {
    /**
     * \brief The basis matrix for the rotation.
     */
    mat3x3 basis;
    
    /**
     * \brief The origin vector translation.
     */
    vec3 origin;
    
    /**
     * \brief A quaternion representing the rotation.
     */
    quat rotation;
    
    /**
     * \brief The OpenGL version of the transform.
     */
    float glMatrix[15];
} rsge_physics_transform_t;

#endif