#include <rsge/gfx/objects/texbuff.h>
#include <log.h>
#include <string.h>

rsge_error_e rsge_obj_texbuff_create(rsge_obj_texbuff_t* texbuff,rsge_obj_texbuff_type_e type,size_t res_w,size_t res_h,rsge_obj_texture_t* startingTexture) {
	if(texbuff->tex > 0) {
		rsge_error_e err = rsge_obj_texbuff_destroy(texbuff);
		if(err != RSGE_ERROR_NONE) return err;
	}
    memset(texbuff,0,sizeof(rsge_obj_texbuff_t));
    
    GLenum target = -1;
	GLint internalFormat = -1;
	GLint format = -1;
	GLint minFilter = -1;
	GLint magFilter = -1;
	GLint clamping = -1;
	
	if(type == RSGE_OBJ_TEXBUFF_TYPE_DEPTH) {
	    target = GL_TEXTURE_2D;
		internalFormat = GL_DEPTH_COMPONENT24;
		format = GL_DEPTH_COMPONENT;
		minFilter = GL_LINEAR;
		magFilter = GL_LINEAR;
		clamping = GL_CLAMP_TO_EDGE;
	} else if(type == RSGE_OBJ_TEXBUFF_TYPE_COLOR) {
	    target = GL_TEXTURE_2D;
		internalFormat = GL_RGBA32F;
		format = GL_RGBA;
		minFilter = GL_LINEAR;
		magFilter = GL_LINEAR;
		clamping = GL_CLAMP_TO_EDGE;
	} else if(type == RSGE_OBJ_TEXBUFF_TYPE_DEPTHCUBE) {
	    target = GL_TEXTURE_CUBE_MAP;
		internalFormat = GL_DEPTH_COMPONENT24;
		format = GL_DEPTH_COMPONENT;
		minFilter = GL_LINEAR;
		magFilter = GL_LINEAR;
		clamping = GL_CLAMP_TO_EDGE;
	} else if(type == RSGE_OBJ_TEXBUFF_TYPE_COLORCUBE) {
	    target = GL_TEXTURE_CUBE_MAP;
		internalFormat = GL_RGBA32F;
		format = GL_RGBA;
		minFilter = GL_LINEAR;
		magFilter = GL_LINEAR;
		clamping = GL_CLAMP_TO_EDGE;
	} else if(type == RSGE_OBJ_TEXBUFF_TYPE_FIXEDTEXTURE) {
	    target = GL_TEXTURE_2D;
		internalFormat = GL_RGB32F;
		format = GL_RGB;
		minFilter = GL_LINEAR_MIPMAP_LINEAR;
		magFilter = GL_LINEAR;
		clamping = GL_REPEAT;
	} else return RSGE_ERROR_INVALID_TYPE;
	
	texbuff->target = target;
	
	glGenTextures(1,&texbuff->tex);
	if(texbuff->tex == 0) {
	    log_error("Failed to create texture");
	    return RSGE_ERROR_OPENGL;
	}
	glBindTexture(texbuff->target,texbuff->tex);
	
	if(type == RSGE_OBJ_TEXBUFF_TYPE_DEPTHCUBE || type == RSGE_OBJ_TEXBUFF_TYPE_COLORCUBE) {
	    for(int i = 0;i < 6;i++) {
	        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,0,internalFormat,res_w,res_h,0,format,GL_FLOAT,startingTexture != NULL ? startingTexture->pixels : NULL);
	    }
	} else {
	    glTexImage2D(texbuff->target,0,internalFormat,res_w,res_h,0,format,GL_FLOAT,startingTexture != NULL ? startingTexture->pixels : NULL);
	}
	
	glTexParameteri(texbuff->target,GL_TEXTURE_MIN_FILTER,minFilter);
	glTexParameteri(texbuff->target,GL_TEXTURE_MAG_FILTER,magFilter);
	glTexParameteri(texbuff->target,GL_TEXTURE_WRAP_R,clamping);
	glTexParameteri(texbuff->target,GL_TEXTURE_WRAP_S,clamping);
	glTexParameteri(texbuff->target,GL_TEXTURE_WRAP_T,clamping);
	
	if(type == RSGE_OBJ_TEXBUFF_TYPE_FIXEDTEXTURE) {
	    glGenerateMipmap(GL_TEXTURE_2D);
		#ifndef GL_TEXTURE_MAX_ANISOTROPY
	    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAX_ANISOTROPY_EXT,16);
		#else
	    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAX_ANISOTROPY,16);
		#endif
	}
    return RSGE_ERROR_NONE;
}

rsge_error_e rsge_obj_texbuff_destroy(rsge_obj_texbuff_t* texbuff) {
    if(texbuff->tex != 0) glDeleteTextures(1,&texbuff->tex);
    memset(texbuff,0,sizeof(rsge_obj_texbuff_t));
    return RSGE_ERROR_NONE;
}
