#include <rsge/gfx/objects/vert.h>
#include <log.h>

rsge_error_e rsge_obj_vert_create(rsge_obj_vert_t* vert,rsge_vertex_t* vertices,size_t vertexCount) {
	if(vert->vao > 0 || vert->buffer > 0) {
		rsge_error_e err = rsge_obj_vert_destroy(vert);
		if(err != RSGE_ERROR_NONE) return err;
	}
	memset(vert,0,sizeof(rsge_obj_vert_t));

	glGenVertexArrays(1,&vert->vao);
	if(vert->vao == 0) {
		log_error("Failed to create a VAO");
		return RSGE_ERROR_OPENGL;
	}
	glBindVertexArray(vert->vao);

	glGenBuffers(1,&vert->buffer);
	if(vert->buffer == 0) {
		glBindVertexArray(0);
		glDeleteVertexArrays(1,&vert->vao);
		log_error("Failed to create buffer");
		return RSGE_ERROR_OPENGL;
	}
	glBindBuffer(GL_ARRAY_BUFFER,vert->buffer);
	glBufferData(GL_ARRAY_BUFFER,vertexCount*sizeof(rsge_vertex_t),vertices,GL_STATIC_DRAW);

	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(rsge_vertex_t),(const void*)((char*)&vertices[0].pos-(char*)&vertices[0]));
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(rsge_vertex_t),(const void*)((char*)&vertices[0].normal-(char*)&vertices[0]));
	glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,sizeof(rsge_vertex_t),(const void*)((char*)&vertices[0].uv-(char*)&vertices[0]));
	glVertexAttribPointer(3,3,GL_FLOAT,GL_FALSE,sizeof(rsge_vertex_t),(const void*)((char*)&vertices[0].tangent-(char*)&vertices[0]));
	glVertexAttribPointer(4,3,GL_FLOAT,GL_FALSE,sizeof(rsge_vertex_t),(const void*)((char*)&vertices[0].binormal-(char*)&vertices[0]));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_obj_vert_destroy(rsge_obj_vert_t* vert) {
    if(vert->vao != 0) glDeleteVertexArrays(1,&vert->vao);
    if(vert->buffer != 0) glDeleteBuffers(1,&vert->buffer);
    memset(vert,0,sizeof(rsge_obj_vert_t));
    return RSGE_ERROR_NONE;
}
