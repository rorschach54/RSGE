#include <rsge/gfx/font/freetype.h>
#include <rsge/settings.h>
#include <log.h>
#include <string.h>

extern FT_Library rsge_freetype_lib;

static rsge_error_e rsge_font_freetype_destroy(rsge_font_t* font) {
	free((rsge_font_freetype_t*)font->impl);
	memset(font,0,sizeof(rsge_font_freetype_t));
	return RSGE_ERROR_NONE;
}

static rsge_error_e rsge_font_freetype_render(rsge_font_t* font,rsge_obj_texture_t* texture,char* text,int color[4]) {
	/* Get the font implementation structure */
	rsge_font_freetype_t* ft = (rsge_font_freetype_t*)font->impl;

	size_t img_w = 0;
	size_t img_h = 0;
	int ftError = 0;
	FT_GlyphSlot g = ft->face->glyph;
	size_t spacing = 10;

	/* Calculate the surface size */
	for(size_t i = 0;i < strlen(text);i++) {
		char c = text[i];

		/* Tell Freetype to get the index of the glyph and to load it */
		FT_UInt glyph_index = FT_Get_Char_Index(ft->face,c);
		ftError = FT_Load_Glyph(ft->face,glyph_index,FT_LOAD_DEFAULT);
		if(ftError) continue;

		/* Tell Freetype to render the glyph */
		ftError = FT_Render_Glyph(ft->face->glyph,FT_RENDER_MODE_NORMAL);
		if(ftError) continue;

		/* Special variables */
		FT_Bitmap* glyph_bitmap = &g->bitmap;
		size_t glyph_x = g->advance.x >> 6;
		size_t glyph_y = g->advance.y >> 6;
		size_t glyph_w = glyph_bitmap->width;
		size_t glyph_h = glyph_bitmap->rows;

		img_w += glyph_w;

		if((img_h-glyph_y) == 0) img_h += glyph_h;

		img_w += glyph_x+spacing;
		img_h += glyph_y;
	}

	/* Create surface */
	rsge_error_e err = rsge_obj_texture_create(texture);
	if(err != RSGE_ERROR_NONE) return err;
	texture->width = img_w;
	texture->height = img_h;
	texture->pixels = malloc(((texture->width+4-(texture->width % 4))*texture->height));
	if(!texture->pixels) {
		log_error("Failed to allocate %d bytes of memory",((texture->width+4-(texture->width % 4))*texture->height));
		return RSGE_ERROR_MALLOC;
	}
	if(font->invert) {
		size_t off = 0;
		for(size_t y = 0;y < texture->height-1;y++) {
			for(size_t x = 0;x < texture->width;x++) {
				rsge_color_rgba_t pixel;
				pixel.red = (float)(color[0]/255);
				pixel.green = (float)(color[1]/255);
				pixel.blue = (float)(color[2]/255);
				pixel.alpha = (float)(color[3]/255);
				texture->pixels[y*texture->width+x] = pixel;
			}
		}
	}

	/* The pen is the cursor */
	size_t pen_x = 0;
	size_t pen_y = 0;

	/* Draw characters onto surface */
	for(size_t i = 0;i < strlen(text);i++) {
		char c = text[i];

		/* Tell Freetype to get the index of the glyph and to load it */
		FT_UInt glyph_index = FT_Get_Char_Index(ft->face,c);
		ftError = FT_Load_Glyph(ft->face,glyph_index,FT_LOAD_DEFAULT);
		if(ftError) continue;

		/* Tell Freetype to render the glyph */
		ftError = FT_Render_Glyph(ft->face->glyph,FT_RENDER_MODE_NORMAL);
		if(ftError) continue;

		/* Special variables */
		FT_Bitmap* glyph_bitmap = &g->bitmap;
		size_t glyph_x = g->advance.x >> 6;
		size_t glyph_y = g->advance.y >> 6;
		size_t glyph_w = glyph_bitmap->width;
		size_t glyph_h = glyph_bitmap->rows;
		size_t glyph_xmax = g->bitmap_left+glyph_bitmap->width;
		size_t glyph_ymax = (img_h-g->bitmap_top)+glyph_bitmap->rows;
		size_t a = 0;
		size_t j = 0;
		size_t p = 0;
		size_t q = 0;

		/* Draw character */
		for(a = g->bitmap_left,p = 0;a < glyph_xmax;a++,p++) {
			for(j = (img_h-g->bitmap_top),q = 0;j < glyph_ymax;j++,q++) {
				if(a < 0 || j < 0 || a >= img_w || j >= img_h) continue;
				size_t x = (a+pen_x);
				size_t y = (j+pen_y);
				rsge_color_rgba_t pixel;

				if(font->invert) {
					pixel.red = (float)(color[0]/255);
					pixel.green = (float)(color[1]/255);
					pixel.blue = (float)(color[2]/255);
					pixel.alpha = (float)(color[3]/255);
					if(glyph_bitmap->buffer[q*glyph_bitmap->width+p] > 0) {
						pixel.red = pixel.green = pixel.blue = pixel.alpha = 0.0f;
					}
				} else {
					pixel.red = pixel.green = pixel.blue = pixel.alpha = 0.0f;
					if(glyph_bitmap->buffer[q*glyph_bitmap->width+p] > 0) {
						pixel.red = (float)(color[0]/255);
						pixel.green = (float)(color[1]/255);
						pixel.blue = (float)(color[2]/255);
						pixel.alpha = (float)(color[3]/255);
					}
				}
				texture->pixels[y*texture->width+x] = pixel;
			}
		}

		/* Move pen */
		pen_x += glyph_x+spacing;
		pen_y += glyph_y;
	}
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_font_freetype_fromFile(rsge_font_t* font,char* path,size_t sz) {
	char* asset_data;
	size_t asset_size;
	rsge_error_e err = rsge_asset_read(path,&asset_data,&asset_size);
	if(err != RSGE_ERROR_NONE) return err;

	rsge_font_freetype_t* ft = malloc(sizeof(rsge_font_freetype_t));
	if(!ft) return RSGE_ERROR_MALLOC;

	int ftError = FT_New_Memory_Face(
                rsge_freetype_lib,
                asset_data,
                asset_size,
		0,
                &ft->face
        );
	if(ftError) {
		free(ft);
		return RSGE_ERROR_FREETYPE;
	}

	font->destroy = rsge_font_freetype_destroy;
	font->render = rsge_font_freetype_render;

	font->impl = (void*)ft;

	int width;
	err = rsge_settings_getint("gfx.res.width",&width);
	if(err != RSGE_ERROR_NONE) return err;
	ftError = FT_Set_Char_Size(((rsge_font_freetype_t*)font->impl)->face,sz*64,0,width,0);
	if(ftError) {
		err = rsge_font_freetype_destroy(font);
		if(err != RSGE_ERROR_NONE) return err;
		return RSGE_ERROR_FREETYPE;
	}
	return RSGE_ERROR_NONE;
}
