#include <rsge/gfx/shader.h>
#include <log.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

rsge_error_e rsge_shader_fromFile(rsge_shader_t* shader,GLenum type,char* path) {
	char* source;
	size_t sourcesz;
	rsge_error_e err = rsge_asset_read(path,&source,&sourcesz);
	if(err != RSGE_ERROR_NONE) return err;
	
	char* header;
	size_t headersz;
	char headerpathtmp[1];
	size_t headerpathsz = sprintf(headerpathtmp,"rsge@shaders/versions/%s/shaderHeader.glsl",glGetString(GL_SHADING_LANGUAGE_VERSION));
	char* headerpath = malloc(headerpathsz);
	if(!headerpath) {
		free(source);
		log_error("Failed to allocate %d bytes of memory",headerpathsz);
		return RSGE_ERROR_MALLOC;
	}
	memset(headerpath,0,headerpathsz);
	sprintf(headerpath,"rsge@shaders/versions/%s/shaderHeader.glsl",glGetString(GL_SHADING_LANGUAGE_VERSION));
	err = rsge_asset_read(headerpath,&header,&headersz);
	free(headerpath);
	if(err != RSGE_ERROR_NONE) {
		free(source);
		return err;
	}
	
	// There is some sort of bug causing malloc to not work when it was tested on a Raspberry Pi 2 Model B
	char datatmp[1];
	size_t datasz = sprintf(datatmp,"%s\n%s",header,source);
	log_debug("Allocation %d bytes of memory",datasz);
	char* data = malloc(datasz);
	if(data == NULL) {
		log_error("Failed to allocate %d bytes of memory",datasz);
		free(header);
		free(source);
		return RSGE_ERROR_MALLOC;
	}
	memset(data,0,datasz);
	log_debug("header size = %d strlen(header) = %d",headersz,strlen(header));
	memcpy(data,header,strlen(header));
	data[strlen(header)+1] = '\n';
	memcpy(data+1+strlen(header),source,strlen(source));
	
	err = rsge_shader_create(shader,type);
	if(err != RSGE_ERROR_NONE) {
		free(header);
		free(source);
		free(data);
		return err;
	}
	err = rsge_shader_compile(shader,data);
	free(header);
	free(source);
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
	if(shader->id == 0) return RSGE_ERROR_SHADER;
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
		
		log_error("%s\n",errorLog);
		
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
