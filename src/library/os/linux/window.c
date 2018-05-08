#include <rsge/gfx/gl.h>
#include <rsge/gfx/image.h>
#include <rsge/assets.h>
#include <rsge/window.h>
#include <log.h>
#include <stdlib.h>

rsge_error_e rsge_window_seticon_fromSurfaces(rsge_surface_t** surfaces,int surfaceCount) {
	/*GLFWwindow* window = glfwGetCurrentContext();
	GLFWimage* images = malloc(sizeof(GLFWimage)*surfaceCount);
	if(!images) return RSGE_ERROR_MALLOC;
	for(int i = 0;i < surfaceCount;i++) {
		images[i].width = surfaces[i]->width;
		images[i].height = surfaces[i]->height;
		images[i].pixels = surfaces[i]->buffer;
	}
	glfwSetWindowIcon(window,surfaceCount,images);*/
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_window_seticon_fromTextures(rsge_obj_texture_t** textures,int textureCount) {
	/*GLFWwindow* window = glfwGetCurrentContext();
	GLFWimage* images = malloc(sizeof(GLFWimage)*textureCount);
	if(!images) return RSGE_ERROR_MALLOC;
	for(int i = 0;i < textureCount;i++) {
		images[i].width = textures[i]->width;
		images[i].height = textures[i]->height;
		images[i].pixels = malloc(((images[i].width*images[i].height)*4)*sizeof(unsigned char));
		if(!images[i].pixels) return RSGE_ERROR_MALLOC;
		for(size_t y = 0;y < images[i].height;y++) {
			for(size_t x = 0;x < images[i].width;x++) {
				rsge_color_rgba_t pixel = textures[i]->pixels[y*images[i].width+x];
				size_t off = 4*(x+y*images[i].width);
				images[i].pixels[off] = (unsigned char)(pixel.red/255);
				images[i].pixels[off+1] = (unsigned char)(pixel.green/255);
				images[i].pixels[off+2] = (unsigned char)(pixel.blue/255);
				images[i].pixels[off+3] = (unsigned char)(pixel.alpha/255);
			}
		}
	}
	glfwSetWindowIcon(window,textureCount,images);*/
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_window_seticon_fromFiles(char** paths,int pathCount) {
	rsge_obj_texture_t** textures = malloc(sizeof(rsge_obj_texture_t)*pathCount);
	if(!textures) return RSGE_ERROR_MALLOC;
	for(int i = 0;i < pathCount;i++) {
		rsge_obj_texture_t texture;
		rsge_error_e err = rsge_image_fromFile(&texture,paths[i]);
		if(err != RSGE_ERROR_NONE) return err;
		textures[i] = &texture;
	}
	return rsge_window_seticon_fromTextures(textures,pathCount);
}