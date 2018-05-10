#include <rsge/gfx/shader.h>
#include <log.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

rsge_error_e rsge_shader_fromFile(rsge_shader_t* shader,GLenum type,char* path) {
	rsge_error_e err = rsge_shader_create(shader,type);
	if(err != RSGE_ERROR_NONE) return err;
	char headerpathtmp[1];
	size_t headerpathsz = sprintf(headerpathtmp,"rsge@shaders/versions/%s/shaderHeader.glsl",glGetString(GL_SHADING_LANGUAGE_VERSION))+1;
	char* headerpath = malloc(headerpathsz);
	if(!headerpath) {
		log_error("Failed to allocate %d bytes of memory",headerpathsz);
		return RSGE_ERROR_MALLOC;
	}
	memset(headerpath,0,headerpathsz);
	sprintf(headerpath,"rsge@shaders/versions/%s/shaderHeader.glsl",glGetString(GL_SHADING_LANGUAGE_VERSION));
	char* source;
	size_t sourcesz;

	rsge_asset_t asset;
	err = rsge_asset_get(&asset,headerpath);
	free(headerpath);
	if(err != RSGE_ERROR_NONE) return err;
	source = malloc(asset.size);
	if(!source) return RSGE_ERROR_MALLOC;
	memset(source,0,asset.size);
	memcpy(source,asset.data,asset.size);
	source[asset.size] = 0;
	sourcesz = asset.size;

	err = rsge_asset_get(&asset,path);
	if(err != RSGE_ERROR_NONE) return err;
	source = realloc(source,sourcesz+asset.size);
	if(!source) return RSGE_ERROR_MALLOC;
	memset(source+sourcesz,0,asset.size);
	memcpy(source+sourcesz,asset.data,asset.size);
	source[sourcesz+asset.size] = 0;
	sourcesz += asset.size;

	glShaderSource(shader->id,1,(const GLchar**)&source,(const GLint*)&sourcesz);
	free(source);
	/* Bug discovered by Spaceboy Ross */
	/* If the functions end with returning rsge_shader_compile(shader,NULL), it will crash */
	err = rsge_shader_compile(shader,NULL);
	if(err != RSGE_ERROR_NONE) return err;
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_shader_create(rsge_shader_t* shader,GLenum type) {
	memset(shader,0,sizeof(rsge_shader_t));
	log_debug("Creating shader");
	shader->id = glCreateShader(type);
	if(shader->id == 0) {
		char* typeStr = "unknown";
		if(type == GL_GEOMETRY_SHADER) typeStr = "GL_GEOMETRY_SHADER";
		if(type == GL_FRAGMENT_SHADER) typeStr = "GL_FRAGMENT_SHADER";
		if(type == GL_VERTEX_SHADER) typeStr = "GL_VERTEX_SHADER";
		log_error("Failed to create shader (type: %s)",typeStr);
		GLenum glErr = glGetError();
		if(glErr != GL_NO_ERROR) log_error("GL: %s",gluErrorString(glErr));
		return RSGE_ERROR_SHADER;
	}
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_shader_destroy(rsge_shader_t* shader) {
	if(shader->id != 0) glDeleteShader(shader->id);
	memset(shader,0,sizeof(rsge_shader_t));
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_shader_compile(rsge_shader_t* shader,char* source) {
	if(source != NULL) {
		log_debug("Setting shader source code");
		glShaderSource(shader->id,1,(const GLchar**)&source,(GLint[1]){ strlen(source) });
	}
	log_debug("Compiling shader");
	glCompileShader(shader->id);
	log_debug("Getting shader compile status");
	GLint status;
	glGetShaderiv(shader->id,GL_COMPILE_STATUS,&status);
	if(!status) return RSGE_ERROR_SHADER;
	log_debug("Shader compiled with no errors");
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_shaderprg_create(rsge_shaderprg_t* prog) {
	if(prog->id > 0) {
		rsge_error_e err = rsge_shaderprg_destroy(prog);
		if(err != RSGE_ERROR_NONE) return err;
	}
	memset(prog,0,sizeof(rsge_shaderprg_t));

	prog->id = glCreateProgram();
	if(prog->id == 0) return RSGE_ERROR_SHADER;
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_shaderprg_destroy(rsge_shaderprg_t* prog) {
	glDeleteProgram(prog->id);
	memset(prog,0,sizeof(rsge_shaderprg_t));
	return RSGE_ERROR_NONE;
}
