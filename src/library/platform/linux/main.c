#include <rsge/gfx/camera.h>
#include <rsge/gfx/gl.h>
#include <rsge/gfx/lighting.h>
#include <rsge/snd/init.h>
#include <rsge/assets.h>
#include <rsge/config.h>
#include <rsge/settings.h>
#include <rsge/game.h>
#include <rsge/misc.h>
#include <rsge/ui.h>
#include <argp.h>
#include <log.h>

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

void error_callback(int error,const char* description) {
	log_error("GLFW: %s",description);
}

void key_callback(GLFWwindow* window,int key,int scancode,int action,int mods) {
	rsge_error_e err;
	rsge_game_t gameinfo;
	rsge_inputmap_key_t data;
	
	if(action == GLFW_PRESS) data.action = RSGE_INPUTMAP_PRESS;
	if(action == GLFW_RELEASE) data.action = RSGE_INPUTMAP_RELEASE;
	if(action == GLFW_REPEAT) data.action = RSGE_INPUTMAP_REPEAT;
	
	data.key = (char*)glfwGetKeyName(key,scancode);
	
	err = rsge_game_getinfo(&gameinfo);
	if(err != RSGE_ERROR_NONE) {
		for(int i = 0;i < gameinfo.inputMapSize;i++) {
			rsge_inputmap_entry_t* entry = &gameinfo.inputMap[i];
			if(entry->type == RSGE_INPUTMAP_INPUT_KB && entry->cb != NULL) entry->cb((void*)&data);
		}
	}
}

void cursor_position_callback(GLFWwindow* window,double xpos,double ypos) {
	rsge_error_e err;
	rsge_game_t gameinfo;
	rsge_inputmap_mouse_t data;
	
	data.x = xpos;
	data.y = ypos;
	data.action = RSGE_INPUTMAP_NONE;
	
	err = rsge_game_getinfo(&gameinfo);
	if(err != RSGE_ERROR_NONE) {
		for(int i = 0;i < gameinfo.inputMapSize;i++) {
			rsge_inputmap_entry_t* entry = &gameinfo.inputMap[i];
			if(entry->type == RSGE_INPUTMAP_INPUT_MOUSE && entry->cb != NULL) entry->cb((void*)&data);
		}
	}
}

void mouse_button_callback(GLFWwindow* window,int button,int action,int mods) {
	rsge_error_e err;
	rsge_game_t gameinfo;
	rsge_inputmap_mouse_t data;
	
	double xpos, ypos;
	glfwGetCursorPos(window,&xpos,&ypos);
	data.x = xpos;
	data.y = ypos;
	data.action = RSGE_INPUTMAP_NONE;
	if(action == GLFW_PRESS) data.action = RSGE_INPUTMAP_PRESS;
	if(action == GLFW_RELEASE) data.action = RSGE_INPUTMAP_RELEASE;
	if(action == GLFW_REPEAT) data.action = RSGE_INPUTMAP_REPEAT;
	
	if(button == GLFW_MOUSE_BUTTON_LEFT) data.button = 1;
	if(button == GLFW_MOUSE_BUTTON_MIDDLE) data.button = 2;
	if(button == GLFW_MOUSE_BUTTON_RIGHT) data.button = 3;
	
	err = rsge_game_getinfo(&gameinfo);
	if(err != RSGE_ERROR_NONE) {
		for(int i = 0;i < gameinfo.inputMapSize;i++) {
			rsge_inputmap_entry_t* entry = &gameinfo.inputMap[i];
			if(entry->type == RSGE_INPUTMAP_INPUT_MOUSE && entry->cb != NULL) entry->cb((void*)&data);
		}
	}
}

void joystick_callback(int joy,int event) {
	rsge_error_e err;
	rsge_game_t gameinfo;
	rsge_inputmap_joystick_t data;
	
	data.name = (char*)glfwGetJoystickName(joy);
	
	if(event == GLFW_CONNECTED) data.event = RSGE_INPUTMAP_JOYSTICK_EVENT_CONN;
	else if(event == GLFW_DISCONNECTED) data.event = RSGE_INPUTMAP_JOYSTICK_EVENT_DISCONN;
	else {
		data.event = RSGE_INPUTMAP_JOYSTICK_EVENT_AXIS_BUTTON;
		
		data.axis = (float*)glfwGetJoystickAxes(joy,&data.axisCount);
		data.button = (unsigned int*)glfwGetJoystickButtons(joy,&data.buttonCount);
	}
}

void fb_resize(GLFWwindow* window,int width,int height) {
	config_setting_set_int(config_lookup(&rsge_libconfig_cfg,"gfx.res.width"),width);
	config_setting_set_int(config_lookup(&rsge_libconfig_cfg,"gfx.res.height"),height);
	rsge_settings_save();

	rsge_camera_reshape(width,height);
}

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
	
	arguments.log_level = "error";
	arguments.show_config = 0;
	arguments.force_res = NULL;
	arguments.force_fullscreen = 0;
	arguments.enable_vulkan = 0;
	argp_parse(&argp,argc,argv,0,0,&arguments);
	
	log_set_level(LOG_INFO);
	
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
		log_debug("CONFIG_SUBPLATFORM = \""CONFIG_SUBPLATFORM_STR"\"");
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

	/* Get game information */
	rsge_game_t gameinfo;
	err = rsge_game_getinfo(&gameinfo);
	if(err != RSGE_ERROR_NONE) {
#if CONFIG_USE_FREETYPE == 1
		FT_Done_FreeType(rsge_freetype_lib);
#endif
		config_destroy(&rsge_libconfig_cfg);
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
			return EXIT_FAILURE;
		}
		char* res_h_str = malloc(strlen(arguments.force_res)-split);
		if(!res_h_str) {
			free(res_w_str);
			log_error("Failed to allocate memory");
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
		config_destroy(&rsge_libconfig_cfg);
		glfwTerminate();
		return EXIT_FAILURE;
	}
	
	if(!glfwVulkanSupported() && arguments.enable_vulkan) {
		log_error("Vulkan is not supported");
		return EXIT_FAILURE;
	}
	
	#if CONFIG_SUBPLATFORM == desktop
	if(glfwVulkanSupported() && arguments.enable_vulkan) {
		log_warn("Experimental Vulkan support is enabled and available.");
		
		log_debug("Getting Vulkan functions");
		PFN_vkCreateInstance pfnCreateInstance = (PFN_vkCreateInstance)glfwGetInstanceProcAddress(NULL,"vkCreateInstance");
		
		VkInstance instance;
		VkResult result = pfnCreateInstance(NULL,NULL,&instance);
		if(result != VK_SUCCESS) {
			log_error("vkCreateInstance failed");
			return EXIT_FAILURE;
		}
		
		PFN_vkGetDeviceProcAddr pfnGetDeviceProcAddr = (PFN_vkGetDeviceProcAddr)glfwGetInstanceProcAddress(instance,"vkGetDeviceProcAddr");
		PFN_vkCreateDevice pfnCreateDevice = (PFN_vkCreateDevice)glfwGetInstanceProcAddress(instance,"vkCreateDevice");
		
		uint32_t extensions_count;
		const char* extensions = glfwGetRequiredInstanceExtensions(&extensions_count);
		
		VkInstanceCreateInfo ici;
		memset(&ici,0,sizeof(ici));
		ici.enabledExtensionCount = extensions_count;
		ici.ppEnabledExtensionNames = extensions;
		
		VkSurfaceKHR surface;
		result = glfwCreateWindowSurface(instance,window,NULL,&surface);
		if(result != VK_SUCCESS) {
			log_error("Failed to create a window surface with Vulkan.");
			return EXIT_FAILURE;
		}
	}
	#endif

	glfwSetKeyCallback(window,key_callback);
	glfwSetFramebufferSizeCallback(window,fb_resize);
	glfwSetCursorPosCallback(window,cursor_position_callback);
	glfwSetMouseButtonCallback(window,mouse_button_callback);
	glfwSetJoystickCallback(joystick_callback);

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
		glfwDestroyWindow(window);
		glfwTerminate();
		return EXIT_FAILURE;
	}
	
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
		config_destroy(&rsge_libconfig_cfg);
		glfwDestroyWindow(window);
		glfwTerminate();
		return EXIT_FAILURE;
	}

	log_info("RSGE: Initializing game");
	/* Initialize game */
	err = rsge_game_init();
	if(err != RSGE_ERROR_NONE) {
#if CONFIG_USE_FREETYPE == 1
		FT_Done_FreeType(rsge_freetype_lib);
#endif
		rsge_ui_deinit();
		rsge_assets_uninit();
		config_destroy(&rsge_libconfig_cfg);
		glfwDestroyWindow(window);
		glfwTerminate();
		return EXIT_FAILURE;
	}

	/* Update game */
	int exitStatus = EXIT_SUCCESS;
	glfwSetTime(0.0);
	while(!glfwWindowShouldClose(window)) {
		int width,height;
		glfwGetFramebufferSize(window,&width,&height);
		rsge_camera_reshape(width,height);
		rsge_camera_update();
		err = rsge_game_update(glfwGetTime());
		if(err != RSGE_ERROR_NONE) {
			exitStatus = EXIT_FAILURE;
			break;
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	log_info("RSGE: Uninitializing game");
	/* Uninitialize game */
	err = rsge_game_uninit();
	if(err != RSGE_ERROR_NONE) {
#if CONFIG_USE_FREETYPE == 1
		FT_Done_FreeType(rsge_freetype_lib);
#endif
		rsge_ui_deinit();
		rsge_assets_uninit();
		config_destroy(&rsge_libconfig_cfg);
		glfwDestroyWindow(window);
		glfwTerminate();
		return EXIT_FAILURE;
	}

	rsge_ui_deinit();
	rsge_audio_uninit();
	rsge_assets_uninit();

#if CONFIG_USE_FREETYPE == 1
	FT_Done_FreeType(rsge_freetype_lib);
#endif
	config_destroy(&rsge_libconfig_cfg);
	glfwDestroyWindow(window);
	glfwTerminate();
	return exitStatus;
}