#include <rsge/gfx/gl.h>
#include <rsge/gfx/image.h>
#include <rsge/input.h>

#ifndef DOXYGEN_SHOULD_SKIP_THIS
rsge_error_e rsge_input_getclipboard(char** str) {
	/*GLFWwindow* window = glfwGetCurrentContext();
	*str = (char*)glfwGetClipboardString(window);*/
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_input_setclipboard(char* str) {
	/*GLFWwindow* window = glfwGetCurrentContext();
	glfwSetClipboardString(window,str);*/
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_input_setmousecursor_fromSurface(rsge_surface_t* surface) {
	/*GLFWwindow* window = glfwGetCurrentContext();

	GLFWimage image;
	image.width = surface->width;
	image.height = surface->height;
	image.pixels = surface->buffer;

	GLFWcursor* cursor = glfwCreateCursor(&image,0,0);
	if(!cursor) return RSGE_ERROR_OPENGL;

	glfwSetCursor(window,cursor);*/
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_input_setmousecursor_fromTexture(rsge_obj_texture_t* texture) {
	/*GLFWwindow* window = glfwGetCurrentContext();

	GLFWimage image;
	image.width = texture->width;
	image.height = texture->height;
	image.pixels = malloc(((image.width*image.height)*4)*sizeof(unsigned char));
	if(!image.pixels) return RSGE_ERROR_MALLOC;
	for(size_t y = 0;y < image.height;y++) {
		for(size_t x = 0;x < image.width;x++) {
			rsge_color_rgba_t pixel = texture->pixels[y*image.width+x];
			size_t off = 4*(x+y*image.width);
			image.pixels[off] = (unsigned char)(pixel.red/255);
     		image.pixels[off+1] = (unsigned char)(pixel.green/255);
			image.pixels[off+2] = (unsigned char)(pixel.blue/255);
			image.pixels[off+3] = (unsigned char)(pixel.alpha/255);
		}
	}

	GLFWcursor* cursor = glfwCreateCursor(&image,0,0);
	if(!cursor) return RSGE_ERROR_OPENGL;

	glfwSetCursor(window,cursor);*/
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_input_setmousecursor_fromFile(char* path) {
	rsge_obj_texture_t texture;
	rsge_error_e err = rsge_image_fromFile(&texture,path);
	if(err != RSGE_ERROR_NONE) return err;
	return rsge_input_setmousecursor_fromTexture(&texture);
}

rsge_error_e rsge_input_setmousemode(rsge_input_mouse_mode_e mode) {
	/*GLFWwindow* window = glfwGetCurrentContext();

	if(mode == RSGE_INPUT_MOUSE_MODE_NORMAL) glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_NORMAL);
	else if(mode == RSGE_INPUT_MOUSE_MODE_LOCK) glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);
	else if(mode == RSGE_INPUT_MOUSE_MODE_HIDDEN) glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_HIDDEN);*/
	return RSGE_ERROR_NONE;
}
#endif
