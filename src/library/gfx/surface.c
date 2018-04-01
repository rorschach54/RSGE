#include <rsge/gfx/gl.h>
#include <rsge/gfx/surface.h>
#include <stdlib.h>

rsge_error_e rsge_surface_create(rsge_surface_t* surface,size_t width,size_t height,size_t bpp) {
	memset(surface,0,sizeof(rsge_surface_t));

	/* Allocates the pixel buffer */
	surface->buffer = malloc(width*height*bpp);
	if(!surface->buffer) return RSGE_ERROR_MALLOC;

	/* Save the width, height, and bpp */
	surface->width = width;
	surface->height = height;
	surface->bpp = bpp;
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_surface_destroy(rsge_surface_t* surface) {
	free(surface->buffer);
	memset(surface,0,sizeof(rsge_surface_t));
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_surface_render(rsge_surface_t* surface,float sx,float sy) {
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1,&tex);
	glBindTexture(GL_TEXTURE_2D,tex);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_ADD);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glPixelStorei(GL_PACK_ALIGNMENT,1);

	if(surface->bpp == 4) glTexImage2D(GL_TEXTURE_2D,0,3,surface->width,surface->height,0,GL_RGBA,GL_UNSIGNED_INT,surface->buffer);
	else if(surface->bpp == 3) glTexImage2D(GL_TEXTURE_2D,0,3,surface->width,surface->height,0,GL_RGB,GL_UNSIGNED_INT,surface->buffer);
	else return RSGE_ERROR_INVALID_BPP;

	float w = surface->width/sx;
	float h = surface->height/sy;

	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);

	glTexCoord2f(0.0f,0.0f);
	glVertex2f(0.0f,h);

	glTexCoord2f(0.0f,1.0f);
	glVertex2f(0.0f,0.0f);

	glTexCoord2f(1.0f,1.0f);
	glVertex2f(w,0.0f);

	glTexCoord2f(1.0f,0.0f);
	glVertex2f(w,h);

	glEnd();
	glDeleteTextures(1,&tex);
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_surface_blit(rsge_surface_t* surface,rsge_surface_t* orig,vec2 pos) {
	for(size_t y = 0;y < orig->height;y++) {
		for(size_t x = 0;x < orig->width;x++) {
			size_t surface_off = surface->bpp*(((pos[0]+x)+(pos[1]+y))*surface->width);
			size_t orig_off = orig->bpp*((x+y)*orig->width);
			if(surface->bpp == 4) {
				if(orig->bpp == 4) {
					surface->buffer[surface_off] = orig->buffer[orig_off];
					surface->buffer[surface_off+1] = orig->buffer[orig_off+1];
					surface->buffer[surface_off+2] = orig->buffer[orig_off+2];
					surface->buffer[surface_off+3] = orig->buffer[orig_off+3];
				} else if(orig->bpp == 3) {
					surface->buffer[surface_off] = orig->buffer[orig_off];
					surface->buffer[surface_off+1] = orig->buffer[orig_off+1];
					surface->buffer[surface_off+2] = orig->buffer[orig_off+2];
				} else return RSGE_ERROR_INVALID_BPP;
			} else if(surface->bpp == 3) {
				if(orig->bpp == 4) {
					surface->buffer[surface_off] = orig->buffer[orig_off];
					surface->buffer[surface_off+1] = orig->buffer[orig_off+1];
					surface->buffer[surface_off+2] = orig->buffer[orig_off+2];
				} else if(orig->bpp == 3) {
					surface->buffer[surface_off] = orig->buffer[orig_off];
					surface->buffer[surface_off+1] = orig->buffer[orig_off+1];
					surface->buffer[surface_off+2] = orig->buffer[orig_off+2];
				} else return RSGE_ERROR_INVALID_BPP;
			} else return RSGE_ERROR_INVALID_BPP;
		}
	}
	return RSGE_ERROR_NONE;
}
