#ifndef __RSGE_GFX_OBJECT_H_
#define __RSGE_GFX_OBJECT_H_ 1

/** \file src/include/rsge/gfx/object.h
 * \brief Object
 * \author Spaceboy Ross
 */

#include <rsge/error.h>
#include <linmath.h>
#include <list.h>
#include <stdbool.h>

/** \struct rsge_object_t src/include/rsge/gfx/object.h rsge/gfx/object.h
 * \brief Object
 */
typedef struct {
    /**
     * \brief The meshes of the object.
     */
    list_t* meshes;
    
    /**
     * \brief The position of the object.
     */
    vec3 pos;
    
    /**
     * \brief The rotation of the object.
     */
    quat rot;
    
    /**
     * \brief The velocity of the object.
     */
    vec3 velocity;
    
    /**
     * \brief The angular velocity of the object.
     */
    quat angularVelocity;
    
    /**
     * \brief The name of the object.
     */
    char* name;
    
    /**
     * \brief Does the object cast shadows.
     */
    bool isShadowCaster;
} rsge_object_t;

#endif