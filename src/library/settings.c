#include <rsge/platform/settings.h>
#include <rsge/settings.h>
#include <stdbool.h>

extern config_t rsge_libconfig_cfg;

rsge_error_e rsge_settings_getlibconfig(config_t* cfg) {
    *cfg = rsge_libconfig_cfg;
    return RSGE_ERROR_NONE;
}

rsge_error_e rsge_settings_loaddefault() {
    config_setting_t* root = config_root_setting(&rsge_libconfig_cfg);
    config_setting_t* cfg_gfx = config_lookup(&rsge_libconfig_cfg,"gfx");
    config_setting_t* cfg_gfx_res = config_lookup(&rsge_libconfig_cfg,"gfx.res");
    config_setting_t* cfg_gfx_res_width = config_lookup(&rsge_libconfig_cfg,"gfx.res.width");
    config_setting_t* cfg_gfx_res_height = config_lookup(&rsge_libconfig_cfg,"gfx.res.height");
    config_setting_t* cfg_gfx_fullscreen = config_lookup(&rsge_libconfig_cfg,"gfx.fullscreen");
    
    if(!cfg_gfx) cfg_gfx = config_setting_add(root,"gfx",CONFIG_TYPE_GROUP);
    if(!cfg_gfx_res) cfg_gfx_res = config_setting_add(cfg_gfx,"res",CONFIG_TYPE_GROUP);
    if(!cfg_gfx_res_width) cfg_gfx_res_width = config_setting_add(cfg_gfx_res,"width",CONFIG_TYPE_INT);
    if(!cfg_gfx_res_height) cfg_gfx_res_height = config_setting_add(cfg_gfx_res,"height",CONFIG_TYPE_INT);
    if(!cfg_gfx_fullscreen) cfg_gfx_fullscreen = config_setting_add(cfg_gfx,"fullscreen",CONFIG_TYPE_BOOL);
    
    config_setting_set_int(cfg_gfx_res_width,640);
    config_setting_set_int(cfg_gfx_res_height,480);
    config_setting_set_bool(cfg_gfx_fullscreen,false);
    return RSGE_ERROR_NONE;
}