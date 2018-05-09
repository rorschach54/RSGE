#include <rsge/gfx/objects/unifbuff.h>
#include <log.h>
#include <stdlib.h>
#include <string.h>

rsge_error_e rsge_obj_unifbuff_create(rsge_obj_unifbuff_t* ub,GLuint binding,char* name,rsge_shaderprg_t* shaderProgs,int shaderProgCount,size_t size) {
    if(ub->ubo > 0) {
        rsge_error_e err = rsge_obj_unifbuff_destroy(ub);
        if(err != RSGE_ERROR_NONE) return err;
    }
    memset(ub,0,sizeof(rsge_obj_unifbuff_t));
    glGenBuffers(1,&ub->ubo);
    if(ub->ubo == 0) {
        log_error("Failed to create a UBO");
        return RSGE_ERROR_OPENGL;
    }
	ub->blockIndexCount = shaderProgCount;
	ub->blockIndexes = malloc(sizeof(GLuint)*ub->blockIndexCount);
	if(!ub->blockIndexes) {
		glDeleteBuffers(1,&ub->ubo);
        log_error("Failed to allocate %d bytes of memory",sizeof(GLuint)*ub->blockIndexCount);
        return RSGE_ERROR_OPENGL;
	}
	for(int i = 0;i < ub->blockIndexCount;i++) ub->blockIndexes[i] = glGetUniformBlockIndex(shaderProgs[i].id,name);
	glBindBufferBase(GL_UNIFORM_BUFFER,binding,ub->ubo);
	glBufferData(GL_UNIFORM_BUFFER,size, NULL,GL_STATIC_DRAW);
	for(int i = 0;i < ub->blockIndexCount;i++) glUniformBlockBinding(shaderProgs[i].id,ub->blockIndexes[i],binding);
    return RSGE_ERROR_NONE;
}

rsge_error_e rsge_obj_unifbuff_destroy(rsge_obj_unifbuff_t* ub) {
    if(ub->ubo != 0) glDeleteBuffers(1,&ub->ubo);
	if(ub->blockIndexCount > 0) free(ub->blockIndexes);
    memset(ub,0,sizeof(rsge_obj_unifbuff_t));
    return RSGE_ERROR_NONE;
}
