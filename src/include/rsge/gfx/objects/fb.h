#ifndef __RSGE_GFX_OBJECTS_FB_H_
#define __RSGE_GFX_OBJECTS_FB_H_ 1

/** \file src/include/rsge/gfx/objects/fb.h
 * \brief Framebuffer Object
 * \author Spaceboy Ross
 */

#include <rsge/gfx/objects/texbuff.h>
#include <rsge/gfx/gl.h>
#include <rsge/error.h>
#include <list.h>

/**
 * \struct rsge_obj_fb_t src/include/rsge/gfx/objects/fb.h rsge/gfx/objects/fb.h
 * \brief Framebuffer Object
 */
typedef struct {
    /**
     * \brief The GL Framebuffer id.
     */
    GLuint fbo;
    
    /**
     * \brief Texture buffers.
     */
    list_t* texBuffs;
} rsge_obj_fb_t;

/**
 * \fn rsge_error_e rsge_obj_fb_create(rsge_obj_fb_t* fb)
 * \brief Creates a new framebuffer.
 * \param[out] fb The pointer to store the fb.
 * \return An error code.
 */
rsge_error_e rsge_obj_fb_create(rsge_obj_fb_t* fb);

/**
 * \fn rsge_error_e rsge_obj_fb_destroy(rsge_obj_fb_t* fb)
 * \brief Destroyes the framebuffer.
 * \param[out] fb The pointer to store the fb.
 * \return An error code.
 */
rsge_error_e rsge_obj_fb_destroy(rsge_obj_fb_t* fb);

#endif