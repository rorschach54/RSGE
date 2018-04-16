#ifndef __RSGE_GFX_FONT_FREETYPE_H_
#define __RSGE_GFX_FONT_FREETYPE_H_ 1

#include <rsge/gfx/font.h>
#include <rsge/assets.h>
#include <rsge/config.h>

#if CONFIG_USE_FREETYPE == 1

#include <ft2build.h>
#include FT_FREETYPE_H

typedef struct {
	FT_Face face;
} rsge_font_freetype_t;

rsge_error_e rsge_font_freetype_fromFile(rsge_font_t* font,rsge_asset_t* asset,size_t sz);

#endif
#endif