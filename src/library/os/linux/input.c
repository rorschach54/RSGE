#include <rsge/gfx/gl.h>
#include <rsge/gfx/image.h>
#include <rsge/input.h>

rsge_error_e rsge_input_getclipboard(char** str) {
	GLFWwindow* window = glfwGetCurrentContext();
	*str = (char*)glfwGetClipboardString(window);
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_input_setclipboard(char* str) {
	GLFWwindow* window = glfwGetCurrentContext();
	glfwSetClipboardString(window,str);
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_input_setmousecursor_fromSurface(rsge_surface_t* surface) {
	GLFWwindow* window = glfwGetCurrentContext();

	GLFWimage image;
	image.width = surface->width;
	image.height = surface->height;
	image.pixels = surface->buffer;

	GLFWcursor* cursor = glfwCreateCursor(&image,0,0);
	if(!cursor) return RSGE_ERROR_OPENGL;

	glfwSetCursor(window,cursor);
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_input_setmousecursor_fromFile(char* path) {
	rsge_surface_t surface;
	rsge_error_e err = rsge_image_fromFile(&surface,path);
	if(err != RSGE_ERROR_NONE) return err;
	return rsge_input_setmousecursor_fromSurface(&surface);
}

rsge_error_e rsge_input_setmousemode(rsge_input_mouse_mode_e mode) {
	GLFWwindow* window = glfwGetCurrentContext();

	if(mode == RSGE_INPUT_MOUSE_MODE_NORMAL) glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_NORMAL);
	else if(mode == RSGE_INPUT_MOUSE_MODE_LOCK) glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);
	else if(mode == RSGE_INPUT_MOUSE_MODE_HIDDEN) glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_HIDDEN);
	return RSGE_ERROR_NONE;
}