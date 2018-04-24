#include <rsge/gfx/gl.h>
#include <rsge/gfx/image.h>
#include <rsge/assets.h>
#include <rsge/window.h>
#include <log.h>
#include <stdlib.h>

rsge_error_e rsge_window_seticon_fromSurfaces(rsge_surface_t** surfaces,int surfaceCount) {
	GLFWwindow* window = glfwGetCurrentContext();
	GLFWimage* images = malloc(sizeof(GLFWimage)*surfaceCount);
	if(!images) return RSGE_ERROR_MALLOC;
	for(int i = 0;i < surfaceCount;i++) {
		images[i].width = surfaces[i]->width;
		images[i].height = surfaces[i]->height;
		images[i].pixels = surfaces[i]->buffer;
	}
	glfwSetWindowIcon(window,surfaceCount,images);
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_window_seticon_fromFiles(char** paths,int pathCount) {
	rsge_surface_t** surfaces = malloc(sizeof(rsge_surface_t)*pathCount);
	if(!surfaces) return RSGE_ERROR_MALLOC;
	for(int i = 0;i < pathCount;i++) {
		rsge_surface_t surface;
		rsge_error_e err = rsge_image_fromFile(&surface,paths[i]);
		if(err != RSGE_ERROR_NONE) return err;
		surfaces[i] = &surface;
	}
	return rsge_window_seticon_fromSurfaces(surfaces,pathCount);
}