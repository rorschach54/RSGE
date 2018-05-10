#include <rsge/gfx/shader.h>
#include <log.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

rsge_error_e rsge_shader_fromFile(rsge_shader_t* shader,GLenum type,char* path) {
	rsge_error_e err = rsge_shader_create(shader,type);
	if(err != RSGE_ERROR_NONE) return err;

	/* Bug discovered by Spaceboy Ross, patched code by Reddit user u/rorschach54 */
	size_t headerpathsz = strlen("rsge@shaders/versions//shaderHeader.glsl")+strlen(glGetString(GL_SHADING_LANGUAGE_VERSION))+1;
	char* headerpath = calloc(0,headerpathsz);
	if(!headerpath) {
		log_error("Failed to allocate %d bytes of memory",headerpathsz);
		return RSGE_ERROR_MALLOC;
	}
	memset(headerpath,0,headerpathsz);
	sprintf(headerpath,"rsge@shaders/versions/%s/shaderHeader.glsl",glGetString(GL_SHADING_LANGUAGE_VERSION));
	char* sources[2];

	size_t sourcesz;
	size_t headersz;

	err = rsge_asset_read(headerpath,&sources[0],&headersz);
	//free(headerpath);
	if(err != RSGE_ERROR_NONE) return err;

	err = rsge_asset_read(path,&sources[1],&sourcesz);
	if(err != RSGE_ERROR_NONE) return err;

	glShaderSource(shader->id,2,(const GLchar**)&sources,(const GLint[2]){
		headersz,
		sourcesz
	});
	for(int i = 0;i < 2;i++) free(sources[i]);
	return rsge_shader_compile(shader,NULL);
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
