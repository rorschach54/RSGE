#include <rsge/gfx/colors.h>
#include <rsge/gfx/gl.h>
#include <rsge/gfx/surface.h>
#include <log.h>
#include <stdlib.h>

rsge_error_e rsge_surface_create(rsge_surface_t* surface,size_t width,size_t height,size_t bpp,uint8_t flags) {
	memset(surface,0,sizeof(rsge_surface_t));

	if(flags & RSGE_SURFACE_FLAG_MIPMAP && flags & RSGE_SURFACE_FLAG_FB) return RSGE_ERROR_INVALID_FLAG;
	if(flags & RSGE_SURFACE_FLAG_WRAP_REPEAT && (flags & RSGE_SURFACE_FLAG_WRAP_MIRROR_REPEAT) ||  (flags & RSGE_SURFACE_FLAG_WRAP_CLAMP)) return RSGE_ERROR_INVALID_FLAG;
	if(flags & RSGE_SURFACE_FLAG_WRAP_MIRROR_REPEAT && (flags & RSGE_SURFACE_FLAG_WRAP_REPEAT) ||  (flags & RSGE_SURFACE_FLAG_WRAP_CLAMP)) return RSGE_ERROR_INVALID_FLAG;
	if(flags & RSGE_SURFACE_FLAG_WRAP_CLAMP && (flags & RSGE_SURFACE_FLAG_WRAP_MIRROR_REPEAT) ||  (flags & RSGE_SURFACE_FLAG_WRAP_REPEAT)) return RSGE_ERROR_INVALID_FLAG;

	/* Save the width, height, and bpp */
	surface->width = width;
	surface->height = height;
	surface->bpp = bpp;
	surface->flags = flags;

	/* Allocates the pixel buffer */
	surface->buffer = malloc(((width*height)*bpp)*sizeof(unsigned char));
	if(!surface->buffer) return RSGE_ERROR_MALLOC;
	return rsge_surface_clear(surface,RSGE_COLOR_BLACK);
}

rsge_error_e rsge_surface_destroy(rsge_surface_t* surface) {
	if(surface->fb != 0) {
		glDeleteFramebuffers(1,&surface->fb);
	}
	if(surface->texture != 0) {
		glDeleteTextures(1,&surface->texture);
	}
	if(surface->list != 0 && !(surface->flags & RSGE_SURFACE_FLAG_MIPMAP)) {
		glDeleteLists(1,surface->list);
	}
	free(surface->buffer);
	memset(surface,0,sizeof(rsge_surface_t));
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_surface_resize(rsge_surface_t* surface,size_t width,size_t height,size_t bpp) {
	if(surface->width != width || surface->height != height || surface->bpp != bpp) {
		/* Store the new width, height, and bpp */
		surface->width = width;
		surface->height = height;
		surface->bpp = bpp;

		/* Free the old buffer and allocate the new buffer */
		free(surface->buffer);
		surface->buffer = malloc(((surface->width*surface->height)*surface->bpp)*sizeof(unsigned char));
		if(!surface->buffer) return RSGE_ERROR_MALLOC;
		return rsge_surface_clear(surface,RSGE_COLOR_BLACK);
	}
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_surface_render(rsge_surface_t* surface,float sx,float sy) {
	if(surface->flags & RSGE_SURFACE_FLAG_FB) {
		if(surface->fb == 0) {
			glGenRenderbuffersEXT(1,&surface->fb);
		}
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT,surface->fb);
		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT,surface->width,surface->height);
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,GL_RENDERBUFFER_EXT,surface->fb);

		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT,0);
	}
	if(surface->texture == 0) {
		glGenTextures(1,&surface->texture);
	}
	glBindTexture(GL_TEXTURE_2D,surface->texture);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

	if(surface->flags & RSGE_SURFACE_FLAG_WRAP_MIRROR_REPEAT) {
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_MIRRORED_REPEAT);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_MIRRORED_REPEAT);
	}

	if(surface->flags & RSGE_SURFACE_FLAG_WRAP_CLAMP) {
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	}

	if(surface->flags & RSGE_SURFACE_FLAG_FB) {
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

		if(surface->bpp == 4) glTexImage2D(GL_TEXTURE_2D,0,3,surface->width,surface->height,0,GL_RGBA,GL_UNSIGNED_BYTE,NULL);
		else if(surface->bpp == 3) glTexImage2D(GL_TEXTURE_2D,0,3,surface->width,surface->height,0,GL_RGB,GL_UNSIGNED_BYTE,NULL);
		else return RSGE_ERROR_INVALID_BPP;

		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT,surface->fb);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_2D,surface->texture,0);
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,GL_RENDERBUFFER_EXT,surface->fb);

		GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
		if(status != GL_FRAMEBUFFER_COMPLETE_EXT) {
			log_error("Failed to create frame buffer");
			return RSGE_ERROR_OPENGL;
		}

		glBindTexture(GL_TEXTURE_2D,0);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT,0);
		return RSGE_ERROR_NONE;
	}

	if(surface->flags & RSGE_SURFACE_FLAG_MIPMAP) {
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

		if(surface->bpp == 4) gluBuild2DMipmaps(GL_TEXTURE_2D,3,surface->width,surface->height,GL_RGBA,GL_UNSIGNED_BYTE,surface->buffer);
		else if(surface->bpp == 3) gluBuild2DMipmaps(GL_TEXTURE_2D,3,surface->width,surface->height,GL_RGB,GL_UNSIGNED_BYTE,surface->buffer);
		else return RSGE_ERROR_INVALID_BPP;
	} else {
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

		if(surface->bpp == 4) glTexImage2D(GL_TEXTURE_2D,0,3,surface->width,surface->height,0,GL_RGBA,GL_UNSIGNED_BYTE,surface->buffer);
		else if(surface->bpp == 3) glTexImage2D(GL_TEXTURE_2D,0,3,surface->width,surface->height,0,GL_RGB,GL_UNSIGNED_BYTE,surface->buffer);
		else return RSGE_ERROR_INVALID_BPP;
	}

	if(!(surface->flags & RSGE_SURFACE_FLAG_MIPMAP)) {
		float w = surface->width*sx;
		float h = surface->height*sy;

		glEnable(GL_TEXTURE_2D);
		if(surface->list == 0) {
			surface->list = glGenLists(1);
			glNewList(surface->list,GL_COMPILE);
		
			glPushMatrix();
			glBegin(GL_QUADS);

			glTexCoord2f(0.0f,1.0f);
			glVertex2f(0.0f,0.0f);

			glTexCoord2f(0.0f,0.0f);
			glVertex2f(0.0f,h);

			glTexCoord2f(1.0f,0.0f);
			glVertex2f(w,h);

			glTexCoord2f(1.0f,1.0f);
			glVertex2f(w,0.0f);

			glEnd();
			glPopMatrix();
			glEndList();
		}
		glPushMatrix();
		glTranslatef(0.0f,0.0f,0.0f);
		glTranslatef(surface->pos[0],surface->pos[1],surface->pos[2]);
		glCallList(surface->list);
		glPopMatrix();
	}
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_surface_clear(rsge_surface_t* surface,int color[4]) {
	for(size_t y = 0;y < surface->height;y++) {
		for(size_t x = 0;x < surface->width;x++) {
			size_t off = surface->bpp*(x+y*surface->width);
			surface->buffer[off] = color[0];
			surface->buffer[off+1] = color[1];
			surface->buffer[off+2] = color[2];
			if(surface->bpp == 4) surface->buffer[off+3] = color[3];
		}
	}
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_surface_blit(rsge_surface_t* surface,rsge_surface_t* orig,vec2 pos) {
	for(size_t y = 0;y < orig->height;y++) {
		for(size_t x = 0;x < orig->width;x++) {
			size_t surface_off = surface->bpp*((pos[0]+x)+(pos[1]+y)*surface->width);
			size_t orig_off = orig->bpp*(x+y*orig->width);
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
