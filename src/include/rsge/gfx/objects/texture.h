#ifndef __RSGE_GFX_OBJECTS_TEXTURE_H_
#define __RSGE_GFX_OBJECTS_TEXTURE_H_ 1

/** \file src/include/rsge/gfx/object/texture.h
 * \brief Texture Object
 * \author Spaceboy Ross
 */

#include <rsge/gfx/colors.h>
#include <rsge/gfx/gl.h>
#include <rsge/error.h>
#include <string.h>

/**
 * \struct rsge_obj_texture_t src/include/rsge/gfx/object/texture.h rsge/gfx/object/texture.h
 * \brief Texture Object
 */
typedef struct {
    /**
     * \brief The name of the texture.
     */
    char* name;
    
    /**
     * \brief The pixel buffer.
     */
    rsge_color_rgba_t* pixels;
    
    /**
     * \brief The width of the texture.
     */
    size_t width;
    
    /**
     * \brief The height of the texture.
     */
    size_t height;
    
    /**
     * \brief The texture ID.
     */
    int id;
} rsge_obj_texture_t;

/**
 * \fn rsge_error_e rsge_obj_texture_create(rsge_obj_texture_t* texture)
 * \brief Creates a new texture.
 * \param[out] texture The pointer to store the texture.
 * \return An error code.
 */
rsge_error_e rsge_obj_texture_create(rsge_obj_texture_t* texture);

/**
 * \fn rsge_error_e rsge_obj_texture_getbyid(int id,rsge_obj_texture_t** texture)
 * \brief Creates a new texture.
 * \param[in] id The ID of the texture.
 * \param[out] texture The pointer to store the texture.
 * \return An error code.
 */
rsge_error_e rsge_obj_texture_getbyid(int id,rsge_obj_texture_t** texture);

#endif