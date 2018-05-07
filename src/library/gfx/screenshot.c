#include <rsge/gfx/screenshot.h>
#include <rsge/settings.h>

rsge_error_e rsge_screenshot_tosurface(rsge_surface_t* surface) {
    int res_w = 0;
    int res_h = 0;
    rsge_error_e err;
    
    err = rsge_settings_getint("gfx.res.width",&res_w);
    if(err != RSGE_ERROR_NONE) return err;
    
    err = rsge_settings_getint("gfx.res.height",&res_h);
    if(err != RSGE_ERROR_NONE) return err;
    
    err = rsge_surface_create(surface,res_w,res_h,4,0);
    if(err != RSGE_ERROR_NONE) return err;
    
    glReadPixels(0,0,res_w,res_h,GL_RGBA,GL_UNSIGNED_BYTE,surface->buffer);
    return RSGE_ERROR_NONE;
}

rsge_error_e rsge_screenshot_totexture(rsge_obj_texture_t* texture) {
    rsge_error_e err = rsge_obj_texture_create(texture);
    if(err != RSGE_ERROR_NONE) return err;
    
    err = rsge_settings_getint("gfx.res.width",(int*)&texture->width);
    if(err != RSGE_ERROR_NONE) return err;
    err = rsge_settings_getint("gfx.res.height",(int*)&texture->height);
    if(err != RSGE_ERROR_NONE) return err;
    
    glReadPixels(0,0,texture->width,texture->height,GL_RGBA,GL_UNSIGNED_BYTE,texture->pixels);
    return RSGE_ERROR_NONE;
}