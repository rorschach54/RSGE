#ifndef __RSGE_GFX_CAMERA_H_
#define __RSGE_GFX_CAMERA_H_ 1

/** \file src/include/rsge/gfx/camera.h
 * \brief Camera stuff
 * \author Spaceboy Ross
 */

#include <rsge/gfx/gl.h>
#include <rsge/error.h>
#include <linmath.h>

/**
 * \fn rsge_error_e rsge_camera_setcolor(int color[4])
 * \brief Sets the color used to clear the frame.
 * \param[in] color The color to use in RGBA format.
 * \return An error.
 */
rsge_error_e rsge_camera_setcolor(int color[4]);

/**
 * \fn rsge_error_e rsge_camera_setrot(vec2 rot)
 * \brief Sets the camera's rotation
 * \param[in] rot The rotation position to use.
 * \return An error code.
 */
rsge_error_e rsge_camera_setrot(vec2 rot);

/**
 * \fn rsge_error_e rsge_camera_setpos(vec3 pos)
 * \brief Sets the camera's position.
 * \param[in] pos The position to use.
 * \return An error code.
 */
rsge_error_e rsge_camera_setpos(vec3 pos);

/**
 * \fn rsge_error_e rsge_camera_getpos(vec3* pos)
 * \brief Gets the camera's position.
 * \param[out] pos The pointer to store the position.
 * \return An error code.
 */
rsge_error_e rsge_camera_getpos(vec3* pos);

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
