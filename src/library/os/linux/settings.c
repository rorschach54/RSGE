#include <rsge/platform/settings.h>
#include <rsge/settings.h>
#include <rsge/game.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern config_t rsge_libconfig_cfg;

rsge_error_e rsge_settings_load() {
	rsge_game_t gameinfo;
	struct passwd* pw = getpwuid(getuid());
	rsge_error_e err = rsge_game_getinfo(&gameinfo);
	if(err != RSGE_ERROR_NONE) return err;
	char* dir = malloc(strlen(pw->pw_dir)+strlen("/.local/share/")+strlen(gameinfo.id)+strlen("/settings.cfg"));
	if(!dir) return RSGE_ERROR_MALLOC;
	strcpy(dir,pw->pw_dir);
	strcat(dir,"/.local/share/");
	strcat(dir,gameinfo.id);
	strcat(dir,"/settings.cfg");

	if(access(dir,F_OK) == -1) {
		err = rsge_settings_save();
		if(err != RSGE_ERROR_NONE) return err;
	}
    
	if(!config_read_file(&rsge_libconfig_cfg,dir)) {
		free(dir);
		return RSGE_ERROR_LIBCONFIG;
	}

	free(dir);
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_settings_save() {
	rsge_game_t gameinfo;
	struct passwd* pw = getpwuid(getuid());
	rsge_error_e err = rsge_game_getinfo(&gameinfo);
	if(err != RSGE_ERROR_NONE) return err;
	char* dir = malloc(strlen(pw->pw_dir)+strlen("/.local/share/")+strlen(gameinfo.id)+strlen("/settings.cfg"));
	if(!dir) return RSGE_ERROR_MALLOC;
	strcpy(dir,pw->pw_dir);
	strcat(dir,"/.local/share/");
	strcat(dir,gameinfo.id);
	strcat(dir,"/settings.cfg");

	if(access(dir,F_OK) == -1) {
		char* subdir = malloc(strlen(pw->pw_dir)+strlen("/.local/share/")+strlen(gameinfo.id));
		if(!subdir) {
			free(dir);
			return RSGE_ERROR_MALLOC;
		}
		strcpy(subdir,pw->pw_dir);
		strcat(subdir,"/.local/share/");
		strcat(subdir,gameinfo.id);

		if(access(subdir,F_OK) == -1) mkdir(subdir,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

		free(subdir);
	}
	config_setting_t* root = config_root_setting(&rsge_libconfig_cfg);
	config_setting_t* cfg_gfx = config_lookup(&rsge_libconfig_cfg,"gfx");
	config_setting_t* cfg_gfx_res = config_lookup(&rsge_libconfig_cfg,"gfx.res");
	config_setting_t* cfg_gfx_res_width = config_lookup(&rsge_libconfig_cfg,"gfx.res.width");
	config_setting_t* cfg_gfx_res_height = config_lookup(&rsge_libconfig_cfg,"gfx.res.height");
	config_setting_t* cfg_gfx_fullscreen = config_lookup(&rsge_libconfig_cfg,"gfx.fullscreen");
    
	if(!cfg_gfx || !cfg_gfx_res || !cfg_gfx_res_width || !cfg_gfx_res_height || !cfg_gfx_fullscreen) {
		err = rsge_settings_loaddefault();
		if(err != RSGE_ERROR_NONE) {
			free(dir);
			return err;
		}
	}
    
	if(!config_write_file(&rsge_libconfig_cfg,dir)) {
		free(dir);
		return RSGE_ERROR_LIBCONFIG;
	}

	free(dir);
	return RSGE_ERROR_NONE;
}
