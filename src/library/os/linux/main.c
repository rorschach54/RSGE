#include <curl/curl.h>
#include <rsge/cl/init.h>
#include <rsge/gfx/camera.h>
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
extern void error_callback(int error,const char* description);
extern void fb_resize(GLFWwindow* window,int width,int height);
extern void rsge_sigint(int dummy);

#if CONFIG_USE_FREETYPE == 1
#include <ft2build.h>
#include FT_FREETYPE_H
#endif

#include <libconfig.h>
#include <stdlib.h>

#if CONFIG_USE_FREETYPE == 1
FT_Library rsge_freetype_lib;
#endif

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

	for(int i = GLFW_JOYSTICK_1;i < GLFW_JOYSTICK_LAST;i++) {
		int present = glfwJoystickPresent(i);
		if(present) {
			const char* name = glfwGetJoystickName(i);
			log_debug("Joystick %d (%s) is present",i+1,name);
		}
	}

	glfwSetErrorCallback(error_callback);
	
	config_init(&rsge_libconfig_cfg);
	err = rsge_settings_load();
	if(err != RSGE_ERROR_NONE) {
		config_destroy(&rsge_libconfig_cfg);
		return EXIT_FAILURE;
	}

	/* Initialize libraries */
	if(!glfwInit()) {
		config_destroy(&rsge_libconfig_cfg);
		return EXIT_FAILURE;
	}

	signal(SIGINT,rsge_sigint);

#if CONFIG_USE_FREETYPE == 1
	if(FT_Init_FreeType(&rsge_freetype_lib)) {
		config_destroy(&rsge_libconfig_cfg);
		glfwTerminate();
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
		glfwTerminate();
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
		glfwTerminate();
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
	GLFWmonitor* monitor = NULL;
	if(config_setting_get_bool(config_lookup(&rsge_libconfig_cfg,"gfx.fullscreen"))) monitor = glfwGetPrimaryMonitor();
	if(arguments.force_fullscreen) monitor = glfwGetPrimaryMonitor();
	log_debug("Setting resolution to %dx%d%s",res_w,res_h,monitor == NULL ? "" : " (Fullscreen)");
	GLFWwindow* window = glfwCreateWindow(res_w,res_h,gameinfo.name,monitor,NULL);
	if(!window) {
#if CONFIG_USE_FREETYPE == 1
		FT_Done_FreeType(rsge_freetype_lib);
#endif
		rsge_audio_uninit();
		curl_global_cleanup();
		config_destroy(&rsge_libconfig_cfg);
		glfwTerminate();
		return EXIT_FAILURE;
	}
	
	if(!glfwVulkanSupported() && arguments.enable_vulkan) {
		log_error("Vulkan is not supported");
#if CONFIG_USE_FREETYPE == 1
		FT_Done_FreeType(rsge_freetype_lib);
#endif
		config_destroy(&rsge_libconfig_cfg);
		rsge_audio_uninit();
		curl_global_cleanup();
		glfwTerminate();
		return EXIT_FAILURE;
	}
	
	#ifdef GLFW_INCLUDE_VULKAN
	if(glfwVulkanSupported() && arguments.enable_vulkan) {
		log_warn("Experimental Vulkan support is enabled and available.");

		VkResult result;
		uint32_t extensions_count;
		const char** extensions = glfwGetRequiredInstanceExtensions(&extensions_count);
		if(!extensions) {
			log_error("Failed to get required extensions");
#if CONFIG_USE_FREETYPE == 1
			FT_Done_FreeType(rsge_freetype_lib);
#endif
			config_destroy(&rsge_libconfig_cfg);
			rsge_audio_uninit();
			curl_global_cleanup();
			glfwTerminate();
			return EXIT_FAILURE;
		}

		PFN_vkEnumerateInstanceLayerProperties pfnEnumerateInstanceLayerProperties = (PFN_vkEnumerateInstanceLayerProperties)glfwGetInstanceProcAddress(NULL,"vkEnumerateInstanceLayerProperties");

		const char** instance_validation_layers = NULL;
		uint32_t instance_layer_count;

		result = pfnEnumerateInstanceLayerProperties(&instance_layer_count,NULL);
		if(result != VK_SUCCESS) {
			log_error("vkEnumerateInstanceLayerProperties failed");
#if CONFIG_USE_FREETYPE == 1
			FT_Done_FreeType(rsge_freetype_lib);
#endif
			config_destroy(&rsge_libconfig_cfg);
			rsge_audio_uninit();
			curl_global_cleanup();
			glfwTerminate();
			return EXIT_FAILURE;
		}

		VkApplicationInfo app = {
			.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
			.pNext = NULL,
			.pApplicationName = gameinfo.name,
			.applicationVersion = 0,
			.pEngineName = "RSGE",
			.engineVersion = 0,
			.apiVersion = VK_API_VERSION_1_0
		};
		VkInstanceCreateInfo inst_info = {
			.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
			.pNext = NULL,
			.pApplicationInfo = &app,
			.enabledLayerCount = 3,
			.ppEnabledLayerNames = (const char**)instance_validation_layers,
			.enabledExtensionCount = extensions_count,
			.ppEnabledExtensionNames = extensions
		};
		
		PFN_vkCreateInstance pfnCreateInstance = (PFN_vkCreateInstance)glfwGetInstanceProcAddress(NULL,"vkCreateInstance");
		
		VkInstance instance;
		result = pfnCreateInstance(&inst_info,NULL,&instance);
		if(result != VK_SUCCESS) {
			log_error("vkCreateInstance failed");
#if CONFIG_USE_FREETYPE == 1
			FT_Done_FreeType(rsge_freetype_lib);
#endif
			config_destroy(&rsge_libconfig_cfg);
			rsge_audio_uninit();
			curl_global_cleanup();
			glfwTerminate();
			return EXIT_FAILURE;
		}
		
		PFN_vkGetDeviceProcAddr pfnGetDeviceProcAddr = (PFN_vkGetDeviceProcAddr)glfwGetInstanceProcAddress(instance,"vkGetDeviceProcAddr");
		PFN_vkCreateDevice pfnCreateDevice = (PFN_vkCreateDevice)glfwGetInstanceProcAddress(instance,"vkCreateDevice");
		
		VkSurfaceKHR surface;
		result = glfwCreateWindowSurface(instance,window,NULL,&surface);
		if(result != VK_SUCCESS) {
			log_error("Failed to create a window surface with Vulkan.");
#if CONFIG_USE_FREETYPE == 1
			FT_Done_FreeType(rsge_freetype_lib);
#endif
			config_destroy(&rsge_libconfig_cfg);
			rsge_audio_uninit();
			curl_global_cleanup();
			glfwTerminate();
			return EXIT_FAILURE;
		}
	}
	#endif

	log_debug("Setting up OpenGL");
	/* Set up OpenGL */
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE,GL_EXP2);
	glHint(GL_FOG_HINT,GL_NICEST);
	
	log_debug("Setting up lighting");
	rsge_lighting_enableLight(0);
	rsge_lighting_enableLight(1);
	vec4 sunpos;
	sunpos[0] = 0.0f;
	sunpos[1] = 0.0f;
	sunpos[2] = 0.0f;
	sunpos[3] = 1.0f;
	rsge_lighting_setpos(0,sunpos);
	
	float ambient[3];
	ambient[0] = 1.0f;
	ambient[1] = 1.0f;
	ambient[2] = 1.0f;
	rsge_lighting_setambient(1,ambient);
	
	float diffuse[3];
	diffuse[0] = 1.0f;
	diffuse[1] = 1.0f;
	diffuse[2] = 1.0f;
	rsge_lighting_setdiffuse(0,diffuse);

	fb_resize(window,res_w,res_h);
	
	log_debug("Assets initializing");
	err = rsge_assets_init();
	if(err != RSGE_ERROR_NONE) {
#if CONFIG_USE_FREETYPE == 1
		FT_Done_FreeType(rsge_freetype_lib);
#endif
		config_destroy(&rsge_libconfig_cfg);
		curl_global_cleanup();
		glfwDestroyWindow(window);
		glfwTerminate();
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
		config_destroy(&rsge_libconfig_cfg);
		glfwDestroyWindow(window);
		glfwTerminate();
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
		config_destroy(&rsge_libconfig_cfg);
		glfwDestroyWindow(window);
		glfwTerminate();
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
		rsge_input_deinit();
		glfwDestroyWindow(window);
		glfwTerminate();
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
		rsge_cl_deinit();
		glfwDestroyWindow(window);
		glfwTerminate();
		return EXIT_FAILURE;
	}

	rsge_callbacks_init();
	
	log_debug("Setting up camera");
	vec3 pos;
	pos[0] = 0.0f;
	pos[1] = 0.0f;
	pos[2] = 0.0f;
	err = rsge_camera_setpos(pos);
	if(err != RSGE_ERROR_NONE) {
#if CONFIG_USE_FREETYPE == 1
		FT_Done_FreeType(rsge_freetype_lib);
#endif
		rsge_ui_deinit();
		rsge_assets_uninit();
		rsge_input_deinit();
		rsge_cl_deinit();
		curl_global_cleanup();
		config_destroy(&rsge_libconfig_cfg);
		glfwDestroyWindow(window);
		glfwTerminate();
		return EXIT_FAILURE;
	}
	
	vec2 rot;
	rot[0] = 0.0f;
	rot[1] = 0.0f;
	err = rsge_camera_setrot(rot);
	if(err != RSGE_ERROR_NONE) {
#if CONFIG_USE_FREETYPE == 1
		FT_Done_FreeType(rsge_freetype_lib);
#endif
		rsge_ui_deinit();
		rsge_assets_uninit();
		rsge_input_deinit();
		curl_global_cleanup();
		rsge_cl_deinit();
		config_destroy(&rsge_libconfig_cfg);
		glfwDestroyWindow(window);
		glfwTerminate();
		return EXIT_FAILURE;
	}

	log_info("RSGE: Initializing game");
	/* Initialize game */
	err = rsge_game_init();
	if(err != RSGE_ERROR_NONE) {
		log_error("rsge_game_init() returned error code %d",err);
#if CONFIG_USE_FREETYPE == 1
		FT_Done_FreeType(rsge_freetype_lib);
#endif
		rsge_ui_deinit();
		rsge_assets_uninit();
		rsge_input_deinit();
		rsge_physics_deinit();
		curl_global_cleanup();
		rsge_cl_deinit();
		config_destroy(&rsge_libconfig_cfg);
		glfwDestroyWindow(window);
		glfwTerminate();
		return EXIT_FAILURE;
	}

	/* Update game */
	int exitStatus = EXIT_SUCCESS;
	double prevTime = glfwGetTime();
	int frameCount = 0;
	do {
		double currentTime = glfwGetTime();
		frameCount++;
		int width,height;
		glfwGetFramebufferSize(window,&width,&height);
		rsge_camera_reshape(width,height);
		rsge_camera_update();
		err = rsge_game_update(currentTime,frameCount);
		if(err != RSGE_ERROR_NONE) {
			exitStatus = EXIT_FAILURE;
			break;
		}
		if(currentTime-prevTime >= 1.0) {
			log_debug("%f ms/frame (FPS: %d)",1000.0/frameCount,frameCount);
			frameCount = 0;
			prevTime += 1.0;
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	} while(!glfwWindowShouldClose(window));

	log_info("RSGE: Uninitializing game");
	/* Uninitialize game */
	err = rsge_game_uninit();
	if(err != RSGE_ERROR_NONE) {
#if CONFIG_USE_FREETYPE == 1
		FT_Done_FreeType(rsge_freetype_lib);
#endif
		rsge_physics_deinit();
		rsge_ui_deinit();
		curl_global_cleanup();
		rsge_cl_deinit();
		rsge_assets_uninit();
		rsge_input_deinit();
		config_destroy(&rsge_libconfig_cfg);
		glfwDestroyWindow(window);
		glfwTerminate();
		return EXIT_FAILURE;
	}

	rsge_ui_deinit();
	rsge_audio_uninit();
	rsge_assets_uninit();
	rsge_input_deinit();
	rsge_physics_deinit();
	rsge_cl_deinit();

#if CONFIG_USE_FREETYPE == 1
	FT_Done_FreeType(rsge_freetype_lib);
#endif
	curl_global_cleanup();
	config_destroy(&rsge_libconfig_cfg);
	glfwDestroyWindow(window);
	glfwTerminate();
	return exitStatus;
}
#endif
