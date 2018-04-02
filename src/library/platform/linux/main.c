#include <rsge/gfx/gl.h>
#include <rsge/snd/init.h>
#include <rsge/config.h>
#include <rsge/settings.h>
#include <rsge/game.h>

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
	// TODO: log_error("GLFW: %s",description);
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

void fb_resize(GLFWwindow* window,int width,int height) {
	GLfloat h = (GLfloat)height/(GLfloat)width;
	GLfloat znear = 5.0f;
	GLfloat zfar = 30.0f;
	GLfloat xmax = znear*0.5f;
	glViewport(0,0,(GLint)width,(GLint)height);
	
	config_setting_set_int(config_lookup(&rsge_libconfig_cfg,"gfx.res.width"),width);
	config_setting_set_int(config_lookup(&rsge_libconfig_cfg,"gfx.res.height"),height);
	rsge_settings_save();
}

int main(char** argv,int argc) {
	/* Variables */
	rsge_error_e err;

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

	/* Create window */
	int res_w = config_setting_get_int(config_lookup(&rsge_libconfig_cfg,"gfx.res.width"));
	int res_h = config_setting_get_int(config_lookup(&rsge_libconfig_cfg,"gfx.res.height"));
	// TODO: add fullscreen
	GLFWwindow* window = glfwCreateWindow(res_w,res_h,gameinfo.name,NULL,NULL);
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

	/* Set up OpenGL */
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);

	/* Initialize game */
	err = rsge_game_init();
	if(err != RSGE_ERROR_NONE) {
#if CONFIG_USE_FREETYPE == 1
		FT_Done_FreeType(rsge_freetype_lib);
#endif
		config_destroy(&rsge_libconfig_cfg);
		glfwDestroyWindow(window);
		glfwTerminate();
		return EXIT_FAILURE;
	}

	/* Update game */
	int exitStatus = EXIT_SUCCESS;
	while(!glfwWindowShouldClose(window)) {
		err = rsge_game_update(glfwGetTime());
		if(err != RSGE_ERROR_NONE) {
			exitStatus = EXIT_FAILURE;
			break;
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	/* Uninitialize game */
	err = rsge_game_uninit();
	if(err != RSGE_ERROR_NONE) {
#if CONFIG_USE_FREETYPE == 1
		FT_Done_FreeType(rsge_freetype_lib);
#endif
		config_destroy(&rsge_libconfig_cfg);
		glfwDestroyWindow(window);
		glfwTerminate();
		return EXIT_FAILURE;
	}

	rsge_audio_uninit();

#if CONFIG_USE_FREETYPE == 1
	FT_Done_FreeType(rsge_freetype_lib);
#endif
	config_destroy(&rsge_libconfig_cfg);
	glfwDestroyWindow(window);
	glfwTerminate();
	return exitStatus;
}
