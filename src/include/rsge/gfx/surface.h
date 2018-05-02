#ifndef __RSGE_GFX_SURFACE_H_
#define __RSGE_GFX_SURFACE_H_ 1

/** \file src/include/rsge/gfx/surface.h
 * \brief Surface stuff
 * \author Spaceboy Ross
 */

#include <rsge/gfx/gl.h>
#include <rsge/error.h>
#include <linmath.h>
#include <stdint.h>
#include <string.h>

/**
 * Enable mipmap generation when rendering.
 */
#define RSGE_SURFACE_FLAG_MIPMAP (1 << 0)
/**
 * Enable frame buffer generation when rendering.
 */
#define RSGE_SURFACE_FLAG_FB (1 << 1)
/**
 * Enable repeating the texture when rendering an object.
 */
#define RSGE_SURFACE_FLAG_WRAP_REPEAT (1 << 2)
/**
 * Enable mirror-repeating the texture when rendering an object.
 */
#define RSGE_SURFACE_FLAG_WRAP_MIRROR_REPEAT (1 << 3)
/**
 * Enable clamping the texture when rendering an object.
 */
#define RSGE_SURFACE_FLAG_WRAP_CLAMP (1 << 4)

/**
 * \struct rsge_surface_t rsge/gfx/surface.h
 * \brief A structure that defines a surface
 */
typedef struct {
	/**
	 * \brief Flags that changes the surface's behaviour.
	 */
	uint8_t flags;

	/**
	 * \brief The width of the surface.
	 */
	size_t width;

	/**
	 * \brief The height of the surface.
	 */
	size_t height;

	/**
	 * \brief The bits per pixel (bpp) of the surface.
	 */
	size_t bpp;

	/**
	 * \brief The pixel buffer of the surface.
	 */
	unsigned char* buffer;

	/**
	 * \brief The texture ID used for rendering.
	 */
	GLuint texture;

	/**
	 * \brief The framebuffer ID used for rendering.
	 */
	GLuint fb;

	/**
	 * \brief The display list ID used for rendering.
	 */
	GLuint list;

	/**
	 * \brief The position to render the surface.
	 */
	vec3 pos;
} rsge_surface_t;

/**
 * \fn rsge_error_e rsge_surface_create(rsge_surface_t* surface,size_t width,size_t height,size_t bpp,uint8_t flags)
 * \brief Creates a new surface.
 * \param[out] surface The pointer to the surface.
 * \param[in] width The width of the surface.
 * \param[in] height The height of the surface.
 * \param[in] bpp The bytes per pixel (bpp) of the surface.
 * \param[in] flags Flags that changes how the surface works.
 * \return An error code.
 */
rsge_error_e rsge_surface_create(rsge_surface_t* surface,size_t width,size_t height,size_t bpp,uint8_t flags);

/**
 * \fn rsge_error_e rsge_surface_destroy(rsge_surface_t* surface)
 * \brief Destroyes a surface.
 * \param[out] surface The pointer to the surface.
 * \return An error code.
 */
rsge_error_e rsge_surface_destroy(rsge_surface_t* surface);

/**
 * \fn rsge_error_e rsge_surface_resize(rsge_surface_t* surface,size_t width,size_t height,size_t bpp)
 * \brief Resizes the surface if needed.
 * \param[out] surface The pointer to the surface.
 * \param[in] width The width of the surface.
 * \param[in] height The height of the surface.
 * \param[in] bpp The bytes per pixel (bpp) of the surface.
 * \return An error code.
 */
rsge_error_e rsge_surface_resize(rsge_surface_t* surface,size_t width,size_t height,size_t bpp);

/**
 * \fn rsge_error_e rsge_surface_render(rsge_surface_t* surface,float sx,float sy)
 * \brief Renders the surface.
 * \param[out] surface The pointer to the surface.
 * \param[in] sx The scale X axis.
 * \param[in] sy The scale Y axis.
 * \return An error code.
 */
rsge_error_e rsge_surface_render(rsge_surface_t* surface,float sx,float sy);

/**
 * \fn rsge_error_e rsge_surface_clear(rsge_surface_t* surface,int color[4])
 * \brief Clears the surface.
 * \param[out] surface The pointer to the surface.
 * \param[in] color The color to use.
 * \return An error code.
 */
rsge_error_e rsge_surface_clear(rsge_surface_t* surface,int color[4]);

/**
 * \fn rsge_error_e rsge_surface_blit(rsge_surface_t* surface,rsge_surface_t* orig,vec2 pos)
 * \brief Blits the original surface onto another surface.
 * \param[out] surface The pointer to the surface.
 * \param[in] orig The original surface to use.
 * \param[in] pos The position to blit the surface at.
 * \return An error code.
 */
rsge_error_e rsge_surface_blit(rsge_surface_t* surface,rsge_surface_t* orig,vec2 pos);

#endif
