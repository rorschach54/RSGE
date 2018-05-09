#include <rsge/gfx/shader.h>
#include <log.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

rsge_error_e rsge_shader_fromFile(rsge_shader_t* shader,GLenum type,char* path) {
	rsge_asset_t sourceAsset;
	rsge_error_e err = rsge_asset_get(&sourceAsset,path);
	if(err != RSGE_ERROR_NONE) return err;
	
	char headerpathtmp[1];
	size_t headerpathsz = sprintf(headerpathtmp,"rsge@shaders/versions/%s/shaderHeader.glsl",glGetString(GL_SHADING_LANGUAGE_VERSION));
	char* headerpath = malloc(headerpathsz);
	if(!headerpath) {
		log_error("Failed to allocate %d bytes of memory",headerpathsz);
		return RSGE_ERROR_MALLOC;
	}
	memset(headerpath,0,headerpathsz);
	sprintf(headerpath,"rsge@shaders/versions/%s/shaderHeader.glsl",glGetString(GL_SHADING_LANGUAGE_VERSION));
	rsge_asset_t headerAsset;
	err = rsge_asset_get(&headerAsset,headerpath);
	free(headerpath);
	if(err != RSGE_ERROR_NONE) return err;
	
	char datatmp[1];
	size_t datasz = strlen(headerAsset.data)+strlen(sourceAsset.data);
	log_debug("Allocation %d bytes of memory",datasz);
	char* data = malloc(datasz);
	if(data == NULL) {
		log_error("Failed to allocate %d bytes of memory",datasz);
		return RSGE_ERROR_MALLOC;
	}
	memset(data,0,datasz);
	strcpy(data,headerAsset.data);
	strcat(data,sourceAsset.data);
	
	err = rsge_shader_create(shader,type);
	if(err != RSGE_ERROR_NONE) {
		free(data);
		return err;
	}
	err = rsge_shader_compile(shader,data);
	free(data);
	return err;
}

rsge_error_e rsge_shader_create(rsge_shader_t* shader,GLenum type) {
	if(shader->id > 0) {
		rsge_error_e err = rsge_shader_destroy(shader);
		if(err != RSGE_ERROR_NONE) return err;
	}
	memset(shader,0,sizeof(rsge_shader_t));

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
	glShaderSource(shader->id,1,(const GLchar**)&source,NULL);
	glCompileShader(shader->id);
	int status;
	glGetShaderiv(shader->id,GL_COMPILE_STATUS,&status);
	if(!status) {
		GLint logSize = 0;
		glGetShaderiv(shader->id,GL_INFO_LOG_LENGTH,&logSize);
		
		char* errorLog = malloc(logSize);
		if(!errorLog) return RSGE_ERROR_MALLOC;
		glGetShaderInfoLog(shader->id,logSize,&logSize,&errorLog[0]);
		
		log_error("Failed to compile shader: \n%s\n",errorLog);
		
		free(errorLog);
		return RSGE_ERROR_SHADER;
	}
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
