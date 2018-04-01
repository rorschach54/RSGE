#include <rsge/gfx/gl.h>
#include <rsge/config.h>
#include <rsge/game.h>

#if CONFIG_USE_FREETYPE == 1
#include <ft2build.h>
#include FT_FREETYPE_H
#endif

#include <stdlib.h>

#if CONFIG_USE_FREETYPE == 1
FT_Library rsge_freetype_lib;
#endif

void error_callback(int error,const char* description) {
	// TODO: log_error("GLFW: %s",description);
}

void fb_resize(GLFWwindow* window,int width,int height) {
	GLfloat h = (GLfloat)height/(GLfloat)width;
	GLfloat znear = 5.0f;
	GLfloat zfar = 30.0f;
	GLfloat xmax = znear*0.5f;
	glViewport(0,0,(GLint)width,(GLint)height);
}

int main(char** argv,int argc) {
	/* Variables */
	rsge_error_e err;

	glfwSetErrorCallback(error_callback);

	/* Initialize libraries */
	if(!glfwInit()) {
		return EXIT_FAILURE;
	}

#if CONFIG_USE_FREETYPE == 1
	if(FT_Init_FreeType(&rsge_freetype_lib)) {
		glfwTerminate();
		return EXIT_FAILURE;
	}
#endif

	/* Get game information */
	rsge_game_t gameinfo;
	err = rsge_game_getinfo(&gameinfo);
	if(err != RSGE_ERROR_NONE) {
#if CONFIG_USE_FREETYPE == 1
		FT_Done_FreeType(rsge_freetype_lib);
#endif
		glfwTerminate();
		return EXIT_FAILURE;
	}

	/* Create window */
	GLFWwindow* window = glfwCreateWindow(640,480,gameinfo.name,NULL,NULL);
	if(!window) {
#if CONFIG_USE_FREETYPE == 1
		FT_Done_FreeType(rsge_freetype_lib);
#endif
		glfwTerminate();
		return EXIT_FAILURE;
	}

	/* Set up OpenGL */
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);
	glfwSetFramebufferSizeCallback(window,fb_resize);

	/* Initialize game */
	err = rsge_game_init();
	if(err != RSGE_ERROR_NONE) {
#if CONFIG_USE_FREETYPE == 1
		FT_Done_FreeType(rsge_freetype_lib);
#endif
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
		glfwDestroyWindow(window);
		glfwTerminate();
		return EXIT_FAILURE;
	}

#if CONFIG_USE_FREETYPE == 1
	FT_Done_FreeType(rsge_freetype_lib);
#endif
	glfwDestroyWindow(window);
	glfwTerminate();
	return exitStatus;
}
