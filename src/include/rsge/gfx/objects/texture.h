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

#endif