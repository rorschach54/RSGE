#ifndef __RSGE_GFX_CAMERA_H_
#define __RSGE_GFX_CAMERA_H_ 1

/** \file src/include/rsge/gfx/camera.h
 * \brief Camera stuff
 * \author Spaceboy Ross
 */

#include <rsge/gfx/gl.h>
#include <rsge/error.h>
#include <linmath.h>

/** \struct rsge_camera_t src/include/rsge/gfx/camera.h rsge/gfx/camera.h
 * \brief A camera
 */
typedef struct {
    /**
     * \brief The Field-of-view.
     */
    vec2 fov;
    
    /**
     * \brief Clip Planes
     */
    vec2 clipPlanes;
    
    /**
     * \brief The camera's rotation.
     */
    quat rot;
    
    /**
     * \brief The camera's position.
     */
    vec3 pos;
} rsge_camera_t;

/**
 * \fn rsge_error_e rsge_camera_getmaincam(rsge_camera_t** cam)
 * \brief Gets the main camera.
 * \param[out] cam The pointer to store the camera.
 * \return An error code.
 */
rsge_error_e rsge_camera_getmaincam(rsge_camera_t** cam);

/**
 * \fn rsge_error_e rsge_camera_create(rsge_camera_t* cam)
 * \brief Creates a new camera.
 * \param[out] cam The pointer to store the camera.
 * \return An error code.
 */
rsge_error_e rsge_camera_create(rsge_camera_t* cam);

#endif
