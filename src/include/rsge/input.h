#ifndef __RSGE_INPUT_H_
#define __RSGE_INPUT_H_ 1

#include <rsge/gfx/surface.h>
#include <rsge/config.h>
#include <rsge/error.h>
#include <libxml/parser.h>
#include <list.h>

typedef enum {
	RSGE_INPUT_DEVICE_TYPE_JOYSTICK,
	RSGE_INPUT_DEVICE_TYPE_MOUSE,
	RSGE_INPUT_DEVICE_TYPE_KEYBOARD,
	RSGE_INPUT_DEVICE_TYPE_PATHDROP
} rsge_input_device_type_e;

typedef enum {
	RSGE_INPUT_DEVICE_AXIS_TYPE_JOYSTICK,
	RSGE_INPUT_DEVICE_AXIS_TYPE_TRIGGER,
	RSGE_INPUT_DEVICE_AXIS_TYPE_DPAD
} rsge_input_device_axis_type_e;

typedef enum {
	RSGE_INPUT_MOUSE_MODE_NORMAL,
	RSGE_INPUT_MOUSE_MODE_LOCK,
	RSGE_INPUT_MOUSE_MODE_HIDDEN
} rsge_input_mouse_mode_e;

typedef struct {
	/**
	 * \brief The type of the axis.
	 */
	rsge_input_device_axis_type_e type;

	/**
	 * \brief The name of the axis.
	 */
	char* name;

	/**
	 * \brief The current value of the axis.
	 */
	float value;
} rsge_input_device_axis_t;

typedef struct {
	/**
	 * \brief The name of the button.
	 */
	char* name;

	/**
	 * \brief The current pressed state of the button.
	 */
	int isPressed;
} rsge_input_device_button_t;

typedef struct {
	/**
	 * \brief The type of the device.
	 */
	rsge_input_device_type_e type;

	/**
	 * \brief The name of the device.
	 */
	char* name;

	union {
		/**
		 * \brief Substructure for a keyboard.
		 */
		struct {
			/**
			 * \brief The current key pressed.
			 */
			char* key;
		} keyboard;

		/**
		 * \brief Substructure for a mouse.
		 */
		struct {
			/**
			 * \brief Position X value.
			 */
			double posX;

			/**
			 * \brief Position Y value.
			 */
			double posY;
			/**
			 * \brief Scroll wheel X value.
			 */
			double scrollX;

			/**
			 * \brief Scroll wheel Y value.
			 */
			double scrollY;

			/**
			 * \brief Left click value.
			 */
			int buttonLeft;

			/**
			 * \brief Middle/scroll click value.
			 */
			int buttonMiddle;

			/**
			 * \brief Right click value.
			 */
			int buttonRight;
		} mouse;

		/**
		 * \brief Substructure for a joystick.
		 */
		struct {
			/**
			 * \brief The number of axes the device has.
			 */
			int axisCount;
		
			/**
			 * \brief The axis map of the device.
			 */
			rsge_input_device_axis_t* axes;
		
			/**
			 * \brief The number of buttons the device has.
			 */
			int buttonCount;
		
			/**
			 * \brief The button map of the device.
			 */
			rsge_input_device_button_t* buttons;
		} joystick;

		/**
		 * \brief Substructure for pathdrop device.
		 */
		struct {
			/**
			 * \brief The number of file paths dropped onto the window.
			 */
			int pathCount;

			/**
			 * \brief The paths.
			 */
			char** paths;
		} pathdrop;
	};

	/**
	 * \brief A list of callbacks.
	 */
	list_t* callbacks;
} rsge_input_device_t;

typedef struct {
	/**
	 * \brief The callback.
	 */
	void (*cb)(rsge_input_device_t*,void*);
	
	/**
	 * \brief The userdata.
	 */
	void* userdata;
} rsge_input_event_t;

/**
 * \fn rsge_error_e rsge_input_init()
 * \brief Initializes the input component of the game engine, this is called automatically.
 * \return An error code.
 */
rsge_error_e rsge_input_init();

/**
 * \fn rsge_error_e rsge_input_deinit()
 * \brief Deinitializes the input component of the game engine, this is called automatically.
 * \return An error code.
 */
rsge_error_e rsge_input_deinit();

/**
 * \fn rsge_error_e rsge_input_getdev_byname(rsge_input_device_t** device,char* name)
 * \brief Gets a device by its name and stores it in device.
 * \param device The pointer to the device.
 * \param name The name of the device to look for.
 * \return An error code.
 */
rsge_error_e rsge_input_getdev_byname(rsge_input_device_t** device,char* name);

/**
 * \fn rsge_error_e rsge_input_registerdev(rsge_input_device_t* device)
 * \brief Registers a device.
 * \param device The device to register.
 * \return An error code.
 */
rsge_error_e rsge_input_registerdev(rsge_input_device_t* device);

/**
 * \fn rsge_error_e rsge_input_registerdev_fromFile(char* path)
 * \brief Registers a device.
 * \param path The path to the XML file.
 * \return An error code.
 */
rsge_error_e rsge_input_registerdev_fromFile(char* path);

/**
 * \fn rsge_error_e rsge_input_emit(rsge_input_device_t* device)
 * \brief Triggers the device's callbacks.
 * \param device The device to trigger the callbacks.
 * \return An error code.
 */
rsge_error_e rsge_input_emit(rsge_input_device_t* device);

/**
 * \fn rsge_error_e rsge_input_addcb(char* devname,void (*cb)(rsge_input_device_t*,void*),void* userdata)
 * \brief Adds a callback to the device.
 * \param devname The name of the device.
 * \param cb The callback function.
 * \param userdata Any extra stuff that is stored.
 * \return An error code.
 */
rsge_error_e rsge_input_addcb(char* devname,void (*cb)(rsge_input_device_t*,void*),void* userdata);

/**
 * \fn rsge_error_e rsge_input_rmcb(char* devname,void (*cb)(rsge_input_device_t*,void*))
 * \brief Removes a callback from the device.
 * \param devname The name of the device.
 * \param cb The callback function.
 * \return An error code.
 */
rsge_error_e rsge_input_rmcb(char* devname,void (*cb)(rsge_input_device_t*,void*));

/**
 * \fn rsge_error_e rsge_input_getclipboard(char** str)
 * \brief Gets the clipboard value and store it in str.
 * \param str The pointer to store the clipboard's value.
 * \return An error code.
 */
rsge_error_e rsge_input_getclipboard(char** str);

/**
 * \fn rsge_error_e rsge_input_setclipboard(char* str)
 * \brief Sets the clipboard value.
 * \param str The string to store in the clipboard.
 * \return An error code.
 */
rsge_error_e rsge_input_setclipboard(char* str);

/**
 * \fn rsge_error_e rsge_input_setmousecursor_fromSurface(rsge_surface_t* surface)
 * \brief Sets the mouse cursor to the surface.
 * \param surface The surface to use as the cursor.
 * \return An error code.
 */
rsge_error_e rsge_input_setmousecursor_fromSurface(rsge_surface_t* surface);

/**
 * \fn rsge_error_e rsge_input_setmousecursor_fromFile(char* path)
 * \brief Sets the mouse cursor to an image from the assets.
 * \param path The path to the image.
 * \return An error code.
 */
rsge_error_e rsge_input_setmousecursor_fromFile(char* path);

/**
 * \fn rsge_error_e rsge_input_setmousemode(rsge_input_mouse_mode_e mode)
 * \brief Sets the mouse mode.
 * \param mode The mouse mode to use.
 * \return An error code.
 */
rsge_error_e rsge_input_setmousemode(rsge_input_mouse_mode_e mode);

#endif