#include <rsge/gfx/objects/unifbuff.h>
#include <log.h>
#include <string.h>

rsge_error_e rsge_obj_unifbuff_create(rsge_obj_unifbuff_t* ub,GLuint binding) {
    memset(ub,0,sizeof(rsge_obj_unifbuff_t));
    glGenBuffers(1,&ub->ubo);
    if(ub->ubo == 0) {
        log_error("Failed to create a UBO");
        return RSGE_ERROR_OPENGL;
    }
    glBindBufferBase(GL_UNIFORM_BUFFER,binding,ub->ubo);
    return RSGE_ERROR_NONE;
}

rsge_error_e rsge_obj_unifbuff_destroy(rsge_obj_unifbuff_t* ub) {
    if(ub->ubo != 0) glDeleteBuffers(1,&ub->ubo);
    memset(ub,0,sizeof(rsge_obj_unifbuff_t));
    return RSGE_ERROR_NONE;
}