#ifndef __RSGE_GFX_FONT_H_
#define __RSGE_GFX_FONT_H_ 1

/** \file src/include/rsge/gfx/font.h
 * \brief Font stuff.
 * \author Spaceboy Ross
 */

#include <rsge/gfx/surface.h>
#include <rsge/assets.h>
#include <rsge/error.h>
#include <stdbool.h>

struct rsge_font;

/**
 * \brief The prototype typedefine for destroying a font.
 */
typedef rsge_error_e (*rsge_font_impl_destroy)(struct rsge_font* font);

/**
 * \brief The prototype typedefine for rendering a font.
 */
typedef rsge_error_e (*rsge_font_impl_render)(struct rsge_font* font,rsge_surface_t* surface,char* text,int color[4]);

/**
 * \struct rsge_font_t rsge/gfx/font.h
 * \brief Defines a font.
 */
typedef struct rsge_font {
	/**
	 * \brief Font backend implementation data.
	 */
	void* impl;

	/**
	 * \brief Font backend implementation destroy function.
	 */
	rsge_font_impl_destroy destroy;

	/**
	 * \brief Font backend implementation render function.
	 */
	rsge_font_impl_render render;

	/**
	 * \brief Inverts the font.
	 */
	bool invert;
} rsge_font_t;

/**
 * \brief Destroyes a font.
 */
#define rsge_font_destroy(font) ((font)->destroy == NULL ? RSGE_ERROR_NONE : (font)->destroy(font))

/**
 * \brief Renders a font.
 */
#define rsge_font_render(font,surface,text,color) ((font)->render == NULL ? RSGE_ERROR_NONE : (font)->render(font,surface,text,color))

/**
 * \fn rsge_error_e rsge_font_fromFile(rsge_font_t* font,char* path,size_t sz)
 * \brief Loads a font from an asset file.
 * \param[out] font The pointer to a font.
 * \param[in] path The path to the asset.
 * \param[in] sz The size to make the font
 * \return An error code.
 */
rsge_error_e rsge_font_fromFile(rsge_font_t* font,char* path,size_t sz);

#endif
