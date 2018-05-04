#ifndef __RSGE_GFX_OBJECTS_TEXBUFF_H_
#define __RSGE_GFX_OBJECTS_TEXBUFF_H_ 1

/** \file src/include/rsge/gfx/object/texbuff.h
 * \brief Texture Buffer
 * \author Spaceboy Ross
 */

#include <rsge/gfx/objects/texture.h>
#include <rsge/gfx/gl.h>
#include <rsge/error.h>

/** \enum rsge_obj_texbuff_type_e
 * \brief Texture Buffer type.
 */
typedef enum {
    /**
     * Depth texture buffer.
     */
    RSGE_OBJ_TEXBUFF_TYPE_DEPTH,
    /**
     * Color texture buffer.
     */
    RSGE_OBJ_TEXBUFF_TYPE_COLOR,
    /**
     * Depth cube texture buffer.
     */
    RSGE_OBJ_TEXBUFF_TYPE_DEPTHCUBE,
    /**
     * Color cube texture buffer.
     */
    RSGE_OBJ_TEXBUFF_TYPE_COLORCUBE,
    /**
     * Fixed texture buffer.
     */
    RSGE_OBJ_TEXBUFF_TYPE_FIXEDTEXTURE
} rsge_obj_texbuff_type_e;

/** \struct rsge_obj_texbuff_t src/include/rsge/gfx/object/texbuff.h rsge/gfx/object/texbuff.h
 * \brief Texture Buffer
 */
typedef struct {
    /**
     * \brief OpenGL texture id.
     */
    GLuint tex;
    
    /**
     * \brief OpenGL texture target.
     */
    GLuint target;
} rsge_obj_texbuff_t;

/**
 * \fn rsge_error_e rsge_obj_texbuff_create(rsge_obj_texbuff_t* texbuff,rsge_obj_texbuff_type_e type,size_t res_w,size_t res_h,rsge_obj_texture_t* startingTexture)
 * \brief Creates a new texture buffer.
 * \param[out] texbuff The pointer to store the texture buffer.
 * \param[in] type The type of texture buffer to create.
 * \param[in] res_w The resolution width.
 * \param[in] res_h The resolution height.
 * \param[in] startingTexture The starting texture.
 * \return An error code.
 */
rsge_error_e rsge_obj_texbuff_create(rsge_obj_texbuff_t* texbuff,rsge_obj_texbuff_type_e type,size_t res_w,size_t res_h,rsge_obj_texture_t* startingTexture);

/**
 * \fn rsge_error_e rsge_obj_texbuff_destroy(rsge_obj_texbuff_t* texbuff)
 * \brief Destroyes the texture buffer.
 * \param[out] texbuff The pointer to store the texture buffer.
 * \return An error code.
 */
rsge_error_e rsge_obj_texbuff_destroy(rsge_obj_texbuff_t* texbuff);

#endif