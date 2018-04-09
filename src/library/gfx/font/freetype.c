#include <rsge/gfx/font/freetype.h>
#include <string.h>

extern FT_Library rsge_freetype_lib;

rsge_error_e rsge_font_freetype_destroy(rsge_font_t* font) {
	free((rsge_font_freetype_t*)font->impl);
	memset(font,0,sizeof(rsge_font_freetype_t));
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_font_freetype_render(rsge_font_t* font,rsge_surface_t* surface,char* text,int color[4]) {
	rsge_font_freetype_t* ft = (rsge_font_freetype_t*)font->impl;
	FT_GlyphSlot g = ft->face->glyph;
	size_t img_w = 0;
	size_t img_h = 0;
	for(size_t i = 0;i < strlen(text);i++) {
		char c = text[i];
		if(text[i] == '\n' || text[i] == '\t') c = ' ';
		if(FT_Load_Char(ft->face,c,FT_LOAD_RENDER)) continue;
		if(text[i] == '\n') img_h += g->bitmap.rows;
		else if(text[i] == '\t') img_w += (g->bitmap.width*4);
		else if(text[i] == '\b' && i > 0 && i < strlen(text)) {
			if(text[i-1] == '\t') img_w -= g->bitmap.width+5;
			else if(text[i-1] == '\n') img_h -= g->bitmap.rows;
		} else {
			img_w += g->bitmap.width;
			if(i < strlen(text)) img_w += 5;
			while(img_h < g->bitmap.rows) img_h++;
		}
	}
	img_h *= 2;

	rsge_error_e err = rsge_surface_create(surface,img_w,img_h*2,4);
	if(err != RSGE_ERROR_NONE) return err;

	size_t offx = 0;
	size_t offy = 0;

	for(size_t i = 0;i < strlen(text);i++) {
		char c = text[i];
		if(text[i] == '\n' || text[i] == '\t') c = ' ';
		if(FT_Load_Char(ft->face,c,FT_LOAD_RENDER)) continue;
		if(text[i] == '\n') {
			offy += g->bitmap.rows;
			offx = 0;
		} else if(text[i] == '\t') offx += g->bitmap.width*4;
		else if(text[i] == '\b' && i > 0 && i < strlen(text)) {
			if(text[i-1] == '\t') offx -= g->bitmap.width+5;
			else if(text[i-1] == '\n') offy -= g->bitmap.rows;
		} else {
			if(i < strlen(text)) offx += 5;
			for(size_t x = 0;x < g->bitmap.width;x++) {
				for(size_t y = 0;y < g->bitmap.rows;y++) {
					size_t off = 4*(offx+(offy+y+((img_h-offy)-g->bitmap_top))*img_w);
					surface->buffer[off] = surface->buffer[off+1] = surface->buffer[off+2] = surface->buffer[off+3] = 0.0f;
					if(g->bitmap.buffer[x+g->bitmap.width*y] > 0) {
						surface->buffer[off] = color[0]/0.01f;
						surface->buffer[off+1] = color[1]/0.01f;
						surface->buffer[off+2] = color[2]/0.01f;
						surface->buffer[off+3] = color[3]/0.01f;
					}
				}
			}
		}
	}
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_font_freetype_fromFile(rsge_font_t* font,rsge_asset_file_t* asset) {
	rsge_font_freetype_t* ft = malloc(sizeof(rsge_font_freetype_t));
	if(!ft) return RSGE_ERROR_MALLOC;

	int ftError = FT_New_Memory_Face(
                rsge_freetype_lib,
                asset->data,
                asset->size,
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
	return RSGE_ERROR_NONE;
}
