#include <rsge/gfx/camera.h>
#include <rsge/gfx/gl.h>
#include <rsge/gfx/lighting.h>
#include <rsge/snd/init.h>
#include <rsge/assets.h>
#include <rsge/config.h>
#include <rsge/settings.h>
#include <rsge/game.h>
#include <rsge/ui.h>
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

#define MACRO2STR(m) #m

int main(char** argv,int argc) {
	/* Variables */
	rsge_error_e err;
	
	log_debug("-- RSGE Configuration:");
	log_debug("CONFIG_PLATFORM = \"%s\"",MACRO2STR(CONFIG_PLATFORM));
	log_debug("CONFIG_AUDIO_BACKEND = \"%s\"",MACRO2STR(CONFIG_AUDIO_BACKEND));
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
	log_debug("-- End of RSGE Configuration");

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
	GLFWmonitor* monitor = NULL;
	if(config_setting_get_bool(config_lookup(&rsge_libconfig_cfg,"gfx.fullscreen"))) monitor = glfwGetPrimaryMonitor();
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