#include <rsge/gfx/camera.h>
#include <rsge/gfx/gl.h>
#include <rsge/input.h>
#include <rsge/settings.h>
#include <log.h>

extern config_t rsge_libconfig_cfg;

void error_callback(int error,const char* description) {
	log_error("GLFW: %s",description);
}

void key_callback(GLFWwindow* window,int key,int scancode,int action,int mods) {
	rsge_input_device_t* device;
	rsge_error_e err = rsge_input_getdev_byname(&device,"keyboard");
	if(err == RSGE_ERROR_NONE) {
		device->keyboard.key = (char*)glfwGetKeyName(key,scancode);

		rsge_input_emit(device);
	}
}

void cursor_position_callback(GLFWwindow* window,double xpos,double ypos) {
	rsge_input_device_t* device;
	rsge_error_e err = rsge_input_getdev_byname(&device,"mouse");
	if(err == RSGE_ERROR_NONE) {
		device->mouse.posX = xpos;
		device->mouse.posY = ypos;

		rsge_input_emit(device);
	}
}

void scroll_callback(GLFWwindow* window,double xoffset,double yoffset) {
	rsge_input_device_t* device;
	rsge_error_e err = rsge_input_getdev_byname(&device,"mouse");
	if(err == RSGE_ERROR_NONE) {
		device->mouse.scrollX = xoffset;
		device->mouse.scrollY = yoffset;

		rsge_input_emit(device);
	}
}

void mouse_button_callback(GLFWwindow* window,int button,int action,int mods) {
	rsge_input_device_t* device;
	rsge_error_e err = rsge_input_getdev_byname(&device,"mouse");
	if(err == RSGE_ERROR_NONE) {
		glfwGetCursorPos(window,&device->mouse.posX,&device->mouse.posY);

		device->mouse.buttonLeft = glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
		device->mouse.buttonMiddle = glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS;
		device->mouse.buttonRight = glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;

		rsge_input_emit(device);
	}
}

void joystick_callback(int joy,int event) {
	log_debug("Joystick %d event occurring");
	rsge_input_device_t* device;
	rsge_error_e err = rsge_input_getdev_byname(&device,(char*)glfwGetJoystickName(joy));
	if(err == RSGE_ERROR_NONE) {
		int axis_count;
		const float* axes = glfwGetJoystickAxes(joy,&axis_count);

		int button_count;
		const unsigned char* buttons = glfwGetJoystickButtons(joy,&button_count);

		for(int i = 0;i < axis_count,i < device->joystick.axisCount;i++) {
			rsge_input_device_axis_t* axis = (rsge_input_device_axis_t*)&device->joystick.axes[i];
			axis->value = axes[i];
		}

		for(int i = 0;i < button_count,i < device->joystick.buttonCount;i++) {
			rsge_input_device_button_t* button = (rsge_input_device_button_t*)&device->joystick.buttons[i];
			button->isPressed == buttons[i] == GLFW_PRESS;
		}

		rsge_input_emit(device);
	}
}

void fb_resize(GLFWwindow* window,int width,int height) {
	config_setting_set_int(config_lookup(&rsge_libconfig_cfg,"gfx.res.width"),width);
	config_setting_set_int(config_lookup(&rsge_libconfig_cfg,"gfx.res.height"),height);
	rsge_settings_save();

	rsge_camera_reshape(width,height);
}

void drop_callback(GLFWwindow* window,int count,const char** paths) {
	rsge_input_device_t* device;
	rsge_error_e err = rsge_input_getdev_byname(&device,"pathdrop");
	if(err == RSGE_ERROR_NONE) {
		device->pathdrop.pathCount = count;
		device->pathdrop.paths = (char**)paths;
		rsge_input_emit(device);
	}
}

void rsge_callbacks_init() {
	GLFWwindow* window = glfwGetCurrentContext();
	glfwSetFramebufferSizeCallback(window,fb_resize);
	glfwSetKeyCallback(window,key_callback);
	glfwSetCursorPosCallback(window,cursor_position_callback);
	glfwSetMouseButtonCallback(window,mouse_button_callback);
	glfwSetJoystickCallback(joystick_callback);
	glfwSetScrollCallback(window,scroll_callback);
	glfwSetDropCallback(window,drop_callback);
}