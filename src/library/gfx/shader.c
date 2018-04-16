#include <rsge/gfx/shader.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

rsge_error_e rsge_shader_fromFile(rsge_shader_t* shader,GLenum type,rsge_asset_t* asset) {
	rsge_error_e err = rsge_shader_create(shader,type);
	if(err != RSGE_ERROR_NONE) return err;
	return rsge_shader_compile(shader,(char*)asset->data);
}

rsge_error_e rsge_shader_create(rsge_shader_t* shader,GLenum type) {
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
		
		fprintf(stderr,"%s\n",errorLog);
		
		free(errorLog);
		return RSGE_ERROR_SHADER;
	}
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_shaderprg_create(rsge_shaderprg_t* prog) {
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
