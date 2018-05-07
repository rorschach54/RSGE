#include <rsge/gfx/objects/fb.h>
#include <log.h>
#include <string.h>

rsge_error_e rsge_obj_fb_create(rsge_obj_fb_t* fb) {
    if(fb->fbo > 0) {
        rsge_error_e err = rsge_obj_fb_destroy(fb);
        if(err != RSGE_ERROR_NONE) return err;
    }
    memset(fb,0,sizeof(rsge_obj_fb_t));
    
    glGenFramebuffers(1,&fb->fbo);
    if(fb->fbo == 0) {
        log_error("Failed to create framebuffer");
        return RSGE_ERROR_OPENGL;
    }
    
    fb->texBuffs = list_new();
    if(!fb->texBuffs) {
        glDeleteFramebuffers(1,&fb->fbo);
        log_error("Failed to create list");
        return RSGE_ERROR_MALLOC;
    }
    return RSGE_ERROR_NONE;
}

rsge_error_e rsge_obj_fb_destroy(rsge_obj_fb_t* fb) {
    list_destroy(fb->texBuffs);
    if(fb->fbo != 0) glDeleteFramebuffers(1,&fb->fbo);
    memset(fb,0,sizeof(rsge_obj_fb_t));
    return RSGE_ERROR_NONE;
}