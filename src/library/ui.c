#include <rsge/settings.h>
#include <rsge/ui.h>
#include <log.h>

#define RSGE_TW_ERROR(e) if(!e) { \
    log_error("AntTweakBar: %s",TwGetLastError()); \
    return RSGE_ERROR_TW; \
}

rsge_error_e rsge_ui_init() {
    rsge_error_e err;
    int screenWidth;
    err = rsge_settings_getint((char*)"gfx.res.width",&screenWidth);
    if(err != RSGE_ERROR_NONE) return err;
    int screenHeight;
    err = rsge_settings_getint((char*)"gfx.res.height",&screenHeight);
    if(err != RSGE_ERROR_NONE) return err;
    
    RSGE_TW_ERROR(TwInit(TW_OPENGL_CORE,NULL));
    RSGE_TW_ERROR(TwWindowSize(screenWidth,screenHeight));
    return RSGE_ERROR_NONE;
}

rsge_error_e rsge_ui_deinit() {
    RSGE_TW_ERROR(TwTerminate());
    return RSGE_ERROR_NONE;
}