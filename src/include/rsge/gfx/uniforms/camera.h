#ifndef __RSGE_GFX_UNIFORMS_CAMERA_H_
#define __RSGE_GFX_UNIFORMS_CAMERA_H_ 1

/** \file src/include/rsge/gfx/uniforms/camera.h
 * \brief Camera uniform
 * \author Spaceboy Ross
 */

#include <rsge/gfx/camera.h>
#include <rsge/error.h>
#include <linmath.h>

/** \struct rsge_unif_cam_t src/include/rsge/gfx/uniforms/camera.h rsge/gfx/uniforms/camera.h
 * \brief Camera uniform
 */
typedef struct {
    /**
     * \brief View Matrix.
     */
    mat4x4 viewMatrix;
    
    /**
     * \brief Inverse View Matrix
     */
    mat4x4 inverseViewMatrix;
    
    /**
     * \brief Projection Matrix
     */
    mat4x4 projectionMatrix;
    
    /**
     * \brief Inverse Projection Matrix
     */
    mat4x4 inverseProjectionMatrix;
    
    /**
     * \brief The Field-of-view.
     */
    vec2 fov;
    
    /**
     * \brief Clip Planes
     */
    vec2 clipPlanes;
} rsge_unif_cam_t;

/**
 * \fn rsge_error_e rsge_unif_cam_create(rsge_unif_cam_t* unif,rsge_camera_t* cam)
 * \brief Creates a uniform camera.
 * \param[out] unif The pointer to store the uniform camera.
 * \param[in] cam The pointer to the camera. NULL for the default camera.
 * \return An error code.
 */
rsge_error_e rsge_unif_cam_create(rsge_unif_cam_t* unif,rsge_camera_t* cam);

#endif