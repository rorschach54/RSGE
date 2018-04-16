#include <rsge/assets/rsge_assets.h>
#include <rsge/gfx/shape.h>
#include <string.h>

extern mat4x4 rsge_camera_mvp;

rsge_error_e rsge_shape_create(rsge_shape_t* shape,rsge_shape_vert_t* vertices,int verticeCount) {
	memset(shape,0,sizeof(rsge_shape_t));
	
	shape->vertices = vertices;
	shape->verticeCount = verticeCount;
	
	/* Create vertex buffer */
	glGenBuffers(1,&shape->vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER,shape->vertexBuffer);
	
	/* Setup vertex buffer */
	glBufferData(GL_ARRAY_BUFFER,sizeof(rsge_shape_vert_t)*shape->verticeCount,shape->vertices,GL_STATIC_DRAW);
	
	/* Load fragment shader */
	rsge_shader_t fragShader;
	rsge_asset_t fragShaderSource;
	rsge_error_e err = rsge_asset_find(&rsge_assets,&fragShaderSource,"shaders/shape/shader.frag");
	if(err != RSGE_ERROR_NONE) return err;
	err = rsge_shader_fromFile(&fragShader,GL_FRAGMENT_SHADER,&fragShaderSource);
	if(err != RSGE_ERROR_NONE) return err;
	
	/* Load vertex shader */
	rsge_shader_t vertShader;
	rsge_asset_t vertShaderSource;
	err = rsge_asset_find(&rsge_assets,&vertShaderSource,"shaders/shape/shader.vert");
	if(err != RSGE_ERROR_NONE) {
		rsge_shader_destroy(&fragShader);
		return err;
	}
	err = rsge_shader_fromFile(&vertShader,GL_VERTEX_SHADER,&vertShaderSource);
	if(err != RSGE_ERROR_NONE) {
		rsge_shader_destroy(&fragShader);
		return err;
	}
	
	/* Create shader program */
	err = rsge_shaderprg_create(&shape->shaderProg);
	if(err != RSGE_ERROR_NONE) {
		rsge_shader_destroy(&fragShader);
		rsge_shader_destroy(&vertShader);
		return err;
	}
	
	/* Attach shaders and link program */
	glAttachShader(shape->shaderProg.id,vertShader.id);
	glAttachShader(shape->shaderProg.id,fragShader.id);
	glLinkProgram(shape->shaderProg.id);
	
	rsge_shader_destroy(&fragShader);
	rsge_shader_destroy(&vertShader);
	
	/* Store uniform locations */
	shape->MVP = glGetUniformLocation(shape->shaderProg.id,"MVP");
	shape->vPos = glGetUniformLocation(shape->shaderProg.id,"vPos");
	shape->vCol = glGetUniformLocation(shape->shaderProg.id,"vCol");
	shape->position = glGetUniformLocation(shape->shaderProg.id,"position");
	
	/* Setup attribute pointers */
	glEnableVertexAttribArray(shape->vPos);
	glVertexAttribPointer(shape->vPos,3,GL_FLOAT,GL_FALSE,sizeof(float)*7,(void*)0);
	
	glEnableVertexAttribArray(shape->vCol);
	glVertexAttribPointer(shape->vCol,4,GL_FLOAT,GL_FALSE,sizeof(float)*7,(void*)(sizeof(float)*3));
	
	/* Unbind stuff */
	glBindBuffer(GL_ARRAY_BUFFER,0);
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_shape_destroy(rsge_shape_t* shape) {
	glDeleteBuffers(1,&shape->vertexBuffer);
	rsge_shaderprg_destroy(&shape->shaderProg);
	memset(shape,0,sizeof(rsge_shape_t));
	return RSGE_ERROR_NONE;
}


rsge_error_e rsge_shape_render(rsge_shape_t* shape) {
	/* Bind vertex buffer and use shader program */
	glUseProgram(shape->shaderProg.id);
	glBindBuffer(GL_ARRAY_BUFFER,shape->vertexBuffer);
	
	/* Load the position into the shader program */
	glUniform3fv(shape->position,1,(const GLfloat*)shape->pos);
	
	/* Load the MVP into the shader program */
	glUniformMatrix4fv(shape->MVP,1,GL_FALSE,(const GLfloat*)rsge_camera_mvp);
	
	/* Draw the shape */
	glDrawArrays(GL_TRIANGLES,0,shape->verticeCount);
	return RSGE_ERROR_NONE;
}