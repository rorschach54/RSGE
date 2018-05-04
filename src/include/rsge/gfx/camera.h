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

/**
 * \fn rsge_error_e rsge_camera_setcolor(int color[4])
 * \brief Sets the color used to clear the frame.
 * \param[in] color The color to use in RGBA format.
 * \return An error.
 */
rsge_error_e rsge_camera_setcolor(int color[4]);

/**
 * \fn rsge_error_e rsge_camera_update()
 * \brief Updates the camera. Do not run this in your code, this is handled automatically.
 * \return An error code.
 */
rsge_error_e rsge_camera_update();

/**
 * \fn rsge_error_e rsge_camera_reshape(int width,int height)
 * \brief Reshapes the frame and camera to work properly with the screen or window. Do not run this in your code, this is handled automatically.
 * \param[in] width The new width.
 * \param[in] height The new height.
 * \return An error code.
 */
rsge_error_e rsge_camera_reshape(int width,int height);

#endif
