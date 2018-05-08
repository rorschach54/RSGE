#include <curl/curl.h>
#include <rsge/cl/init.h>
#include <rsge/gfx/camera.h>
#include <rsge/gfx/elglr.h>
#include <rsge/gfx/gl.h>
#include <rsge/gfx/lighting.h>
#include <rsge/physics/init.h>
#include <rsge/snd/init.h>
#include <rsge/assets.h>
#include <rsge/config.h>
#include <rsge/settings.h>
#include <rsge/game.h>
#include <rsge/input.h>
#include <rsge/misc.h>
#include <rsge/ui.h>
#include <argp.h>
#include <log.h>
#include <signal.h>

#ifndef DOXYGEN_SHOULD_SKIP_THIS
extern void rsge_callbacks_init();
extern void fb_resize(int width,int height);
extern void rsge_sigint(int dummy);

extern rsge_elglr_t rsge_elglr;

#if CONFIG_USE_FREETYPE == 1
#include <ft2build.h>
#include FT_FREETYPE_H
#endif

#include <libconfig.h>
#include <stdlib.h>

#if CONFIG_USE_FREETYPE == 1
FT_Library rsge_freetype_lib;
#endif

SDL_Window* sdl_window;
SDL_GLContext sdl_context;
config_t rsge_libconfig_cfg;

static struct argp_option rsge_options[] = {
	{ "log-level", 'l', "LEVEL", 0, "Sets the log level." },
	{ "show-config", 'c', 0, 0, "Show the configuration of the game engine." },
	{ "force-res", 'r', "WIDTHxHEIGHT", 0, "Forces a resolution instead of the one from the settings." },
	{ "force-fullscreen", 'f', 0, 0, "Forces the window to be in fullscreen." },
	{ "enable-vulkan", 'v', 0, 0, "Forces experimental Vulkan support, use at your own risk." },
	{ 0 }
};

struct arguments {
	char* log_level;
	int show_config;
	char* force_res;
	int force_fullscreen;
	int enable_vulkan;
};

static error_t rsge_parse_opt(int key,char* arg,struct argp_state* state) {
	struct arguments* arguments = state->input;
	switch(key) {
		case 'l':
			arguments->log_level = arg;
			break;
		case 'c':
			arguments->show_config = 1;
			break;
		case 'r':
			arguments->force_res = arg;
			break;
		case 'f':
			arguments->force_fullscreen = 1;
			break;
		case 'v':
			arguments->enable_vulkan = 1;
			break;
		case ARGP_KEY_ARG: break;
		case ARGP_KEY_END: break;
		default: return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

static struct argp argp = { rsge_options, rsge_parse_opt, "", "" };

int main(int argc,char** argv) {
	/* Variables */
	rsge_error_e err;
	struct arguments arguments;
	
	arguments.log_level = "info";
	arguments.show_config = 0;
	arguments.force_res = NULL;
	arguments.force_fullscreen = 0;
	arguments.enable_vulkan = 0;
	argp_parse(&argp,argc,argv,0,0,&arguments);
	
	if(!strcmp(arguments.log_level,"debug")) log_set_level(LOG_DEBUG);
	else if(!strcmp(arguments.log_level,"trace")) log_set_level(LOG_TRACE);
	else if(!strcmp(arguments.log_level,"info")) log_set_level(LOG_INFO);
	else if(!strcmp(arguments.log_level,"warn")) log_set_level(LOG_WARN);
	else if(!strcmp(arguments.log_level,"fatal")) log_set_level(LOG_FATAL);
	else {
		if(!!strcmp(arguments.log_level,"?")) printf("Invalid log level: %s\n",arguments.log_level);
		printf("Log Level values:\n");
		printf("\t?\tShows this help\n");
		printf("\tdebug\tEnabled debug mode\n");
		printf("\ttrace\tEnables trace mode\n");
		printf("\tinfo\tEnables info mode\n");
		printf("\twarn\tEnables warn mode\n");
		printf("\tfatal\tEnables fatal mode\n");
		return EXIT_SUCCESS;
	}
	
	if(arguments.show_config) {
		log_debug("-- RSGE Configuration --");
		log_debug("CONFIG_PLATFORM = \""CONFIG_PLATFORM_STR"\"");
		log_debug("CONFIG_AUDIO_BACKEND = \""CONFIG_AUDIO_BACKEND_STR"\"");
		log_debug("CONFIG_OS = \""CONFIG_OS_STR"\"");
		#if CONFIG_USE_FREETYPE == 1
		log_debug("Freetype2 is enabled");
		#else
		log_debug("Freetype2 is disabled");
		#endif
		#if CONFIG_USE_LIBPNG == 1
		log_debug("libpng is enabled");
		#else
		log_debug("libpng is disabled");
		#endif
		log_debug("-- End of RSGE Configuration --");
	}
	
	config_init(&rsge_libconfig_cfg);
	err = rsge_settings_load();
	if(err != RSGE_ERROR_NONE) {
		config_destroy(&rsge_libconfig_cfg);
		return EXIT_FAILURE;
	}

	/* Initialize libraries */
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		config_destroy(&rsge_libconfig_cfg);
		return EXIT_FAILURE;
	}

	signal(SIGINT,rsge_sigint);

#if CONFIG_USE_FREETYPE == 1
	if(FT_Init_FreeType(&rsge_freetype_lib)) {
		config_destroy(&rsge_libconfig_cfg);
		SDL_Quit();
		return EXIT_FAILURE;
	}
#endif

	log_debug("Audio initializing");
	err = rsge_audio_init();
	if(err != RSGE_ERROR_NONE) {
#if CONFIG_USE_FREETYPE == 1
		FT_Done_FreeType(rsge_freetype_lib);
#endif
		config_destroy(&rsge_libconfig_cfg);
		SDL_Quit();
		return EXIT_FAILURE;
	}

	curl_global_init(CURL_GLOBAL_ALL);

	/* Get game information */
	rsge_game_t gameinfo;
	err = rsge_game_getinfo(&gameinfo);
	if(err != RSGE_ERROR_NONE) {
#if CONFIG_USE_FREETYPE == 1
		FT_Done_FreeType(rsge_freetype_lib);
#endif
		config_destroy(&rsge_libconfig_cfg);
		rsge_audio_uninit();
		curl_global_cleanup();
		SDL_Quit();
		return EXIT_FAILURE;
	}
	
	log_debug("Game Name: \"%s\"",gameinfo.name);

	log_debug("Creating window");
	/* Create window */
	int res_w = config_setting_get_int(config_lookup(&rsge_libconfig_cfg,"gfx.res.width"));
	int res_h = config_setting_get_int(config_lookup(&rsge_libconfig_cfg,"gfx.res.height"));
	if(arguments.force_res != NULL) {
		int split = 0;
		for(int i = 0;i < strlen(arguments.force_res);i++) {
			if(arguments.force_res[i] == 'x') {
				split = i;
				break;
			}
		}
		char* res_w_str = malloc(split-1);
		if(!res_w_str) {
			log_error("Failed to allocate memory");
			curl_global_cleanup();
			return EXIT_FAILURE;
		}
		char* res_h_str = malloc(strlen(arguments.force_res)-split);
		if(!res_h_str) {
			free(res_w_str);
			log_error("Failed to allocate memory");
			curl_global_cleanup();
			return EXIT_FAILURE;
		}
		
		memcpy(res_w_str,arguments.force_res,split-1);
		memcpy(res_h_str,arguments.force_res+split,strlen(arguments.force_res)-split);
		
		res_w = atoi(res_w_str);
		res_h = atoi(res_h_str);
		
		free(res_w_str);
		free(res_h_str);
	}
	int sdl_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
	if(config_setting_get_bool(config_lookup(&rsge_libconfig_cfg,"gfx.fullscreen")) || arguments.force_fullscreen) {
		sdl_flags |= SDL_WINDOW_FULLSCREEN;
	}
	log_debug("Setting resolution to %dx%d%s",res_w,res_h,sdl_flags & SDL_WINDOW_FULLSCREEN ? " (Fullscreen)" : "");
	sdl_window = SDL_CreateWindow(gameinfo.name,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,res_w,res_h,sdl_flags);
	if(!sdl_window) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Window creation error",SDL_GetError(),NULL);
#if CONFIG_USE_FREETYPE == 1
		FT_Done_FreeType(rsge_freetype_lib);
#endif
		rsge_audio_uninit();
		curl_global_cleanup();
		config_destroy(&rsge_libconfig_cfg);
		SDL_Quit();
		return EXIT_FAILURE;
	}
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,SDL_GL_CONTEXT_PROFILE_CORE);
	sdl_context = SDL_GL_CreateContext(sdl_window);

	log_debug("Setting up OpenGL");
	/* Set up OpenGL */
	GLenum glErr = glewInit();
	if(glErr != GLEW_OK) {
		log_error("GLEW: %s",glewGetErrorString(glErr));
#if CONFIG_USE_FREETYPE == 1
		FT_Done_FreeType(rsge_freetype_lib);
#endif
		config_destroy(&rsge_libconfig_cfg);
		curl_global_cleanup();
		SDL_GL_DeleteContext(sdl_context);
		SDL_DestroyWindow(sdl_window);
		SDL_Quit();
		return EXIT_FAILURE;
	}
	
	log_debug("Assets initializing");
	err = rsge_assets_init();
	if(err != RSGE_ERROR_NONE) {
#if CONFIG_USE_FREETYPE == 1
		FT_Done_FreeType(rsge_freetype_lib);
#endif
		config_destroy(&rsge_libconfig_cfg);
		curl_global_cleanup();
		SDL_GL_DeleteContext(sdl_context);
		SDL_DestroyWindow(sdl_window);
		SDL_Quit();
		return EXIT_FAILURE;
	}

	fb_resize(res_w,res_h);
	
	log_debug("Initializing ELGLR");
	err = rsge_elglr_init();
	if(err != RSGE_ERROR_NONE) {
#if CONFIG_USE_FREETYPE == 1
		FT_Done_FreeType(rsge_freetype_lib);
#endif
		config_destroy(&rsge_libconfig_cfg);
		curl_global_cleanup();
		rsge_assets_uninit();
		SDL_GL_DeleteContext(sdl_context);
		SDL_DestroyWindow(sdl_window);
		SDL_Quit();
		return EXIT_FAILURE;
	}
	
	log_debug("OpenCL initializing");
	err = rsge_cl_init();
	if(err != RSGE_ERROR_NONE) {
#if CONFIG_USE_FREETYPE == 1
		FT_Done_FreeType(rsge_freetype_lib);
#endif
		rsge_assets_uninit();
		curl_global_cleanup();
		rsge_elglr_deinit();
		config_destroy(&rsge_libconfig_cfg);
		SDL_GL_DeleteContext(sdl_context);
		SDL_DestroyWindow(sdl_window);
		SDL_Quit();
		return EXIT_FAILURE;
	}
	
	log_debug("Input initializing");
	err = rsge_input_init();
	if(err != RSGE_ERROR_NONE) {
#if CONFIG_USE_FREETYPE == 1
		FT_Done_FreeType(rsge_freetype_lib);
#endif
		rsge_assets_uninit();
		rsge_cl_deinit();
		curl_global_cleanup();
		rsge_elglr_deinit();
		config_destroy(&rsge_libconfig_cfg);
		SDL_GL_DeleteContext(sdl_context);
		SDL_DestroyWindow(sdl_window);
		SDL_Quit();
		return EXIT_FAILURE;
	}
	
	log_debug("Physics initializing");
	err = rsge_physics_init();
	if(err != RSGE_ERROR_NONE) {
#if CONFIG_USE_FREETYPE == 1
		FT_Done_FreeType(rsge_freetype_lib);
#endif
		config_destroy(&rsge_libconfig_cfg);
		rsge_assets_uninit();
		curl_global_cleanup();
		rsge_cl_deinit();
		rsge_elglr_deinit();
		rsge_input_deinit();
		SDL_GL_DeleteContext(sdl_context);
		SDL_DestroyWindow(sdl_window);
		SDL_Quit();
		return EXIT_FAILURE;
	}
	
	log_debug("UI initializing");
	err = rsge_ui_init();
	if(err != RSGE_ERROR_NONE) {
#if CONFIG_USE_FREETYPE == 1
		FT_Done_FreeType(rsge_freetype_lib);
#endif
		config_destroy(&rsge_libconfig_cfg);
		rsge_assets_uninit();
		curl_global_cleanup();
		rsge_input_deinit();
		rsge_physics_deinit();
		rsge_elglr_deinit();
		rsge_cl_deinit();
		SDL_GL_DeleteContext(sdl_context);
		SDL_DestroyWindow(sdl_window);
		SDL_Quit();
		return EXIT_FAILURE;
	}

	rsge_callbacks_init();

	log_info("RSGE: Initializing game");
	/* Initialize game */
	err = rsge_game_init(&rsge_elglr);
	if(err != RSGE_ERROR_NONE) {
		log_error("rsge_game_init() returned error code %d",err);
#if CONFIG_USE_FREETYPE == 1
		FT_Done_FreeType(rsge_freetype_lib);
#endif
		rsge_ui_deinit();
		rsge_assets_uninit();
		rsge_elglr_deinit();
		rsge_input_deinit();
		rsge_physics_deinit();
		curl_global_cleanup();
		rsge_cl_deinit();
		config_destroy(&rsge_libconfig_cfg);
		SDL_GL_DeleteContext(sdl_context);
		SDL_DestroyWindow(sdl_window);
		SDL_Quit();
		return EXIT_FAILURE;
	}

	/* Update game */
	int exitStatus = EXIT_SUCCESS;
	int lastTicks = SDL_GetTicks();
	int ticks = lastTicks;
	int frameCount = 0;
	do {
		ticks = SDL_GetTicks();
		err = rsge_game_update(&rsge_elglr,ticks-lastTicks,ticks);
		if(err != RSGE_ERROR_NONE) {
			exitStatus = EXIT_FAILURE;
			break;
		}
		SDL_GL_SwapWindow(sdl_window);
		int ticksToWait = fmax(0,(1000 / 60)-(SDL_GetTicks()-ticks));
		SDL_Delay(ticksToWait);
		lastTicks = ticks;
	} while(true);

	log_info("RSGE: Uninitializing game");
	/* Uninitialize game */
	err = rsge_game_uninit(&rsge_elglr);
	if(err != RSGE_ERROR_NONE) {
#if CONFIG_USE_FREETYPE == 1
		FT_Done_FreeType(rsge_freetype_lib);
#endif
		rsge_physics_deinit();
		rsge_ui_deinit();
		curl_global_cleanup();
		rsge_elglr_deinit();
		rsge_cl_deinit();
		rsge_assets_uninit();
		rsge_input_deinit();
		config_destroy(&rsge_libconfig_cfg);
		SDL_GL_DeleteContext(sdl_context);
		SDL_DestroyWindow(sdl_window);
		SDL_Quit();
		return EXIT_FAILURE;
	}

	rsge_ui_deinit();
	rsge_audio_uninit();
	rsge_assets_uninit();
	rsge_input_deinit();
	rsge_physics_deinit();
	rsge_cl_deinit();
	rsge_elglr_deinit();

#if CONFIG_USE_FREETYPE == 1
	FT_Done_FreeType(rsge_freetype_lib);
#endif
	curl_global_cleanup();
	config_destroy(&rsge_libconfig_cfg);
	SDL_GL_DeleteContext(sdl_context);
	SDL_DestroyWindow(sdl_window);
	SDL_Quit();
	return exitStatus;
}
#endif
