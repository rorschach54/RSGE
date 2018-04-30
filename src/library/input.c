#include <rsge/assets.h>
#include <rsge/input.h>
#include <list.h>
#include <log.h>
#include <string.h>

list_t* rsge_input_devices;

static int rsge_input_findcb(void (*cb)(rsge_input_device_t*,void*),rsge_input_event_t* event) {
	return event->cb == cb;
}

rsge_error_e rsge_input_init() {
	rsge_input_devices = list_new();
	if(!rsge_input_devices) return RSGE_ERROR_MALLOC;

	/* Register devices */
	rsge_error_e err;

	err = rsge_input_registerdev_fromFile("rsge@input/devices/keyboard.xml");
	if(err != RSGE_ERROR_NONE) return err;

	err = rsge_input_registerdev_fromFile("rsge@input/devices/mouse.xml");
	if(err != RSGE_ERROR_NONE) return err;

	err = rsge_input_registerdev_fromFile("rsge@input/devices/pathdrop.xml");
	if(err != RSGE_ERROR_NONE) return err;

	err = rsge_input_registerdev_fromFile("rsge@input/devices/xbox360.xml");
	if(err != RSGE_ERROR_NONE) return err;
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_input_deinit() {
	list_destroy(rsge_input_devices);
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_input_getdev_byname(rsge_input_device_t** device,char* name) {
	list_node_t* node;
	list_iterator_t* it = list_iterator_new(rsge_input_devices,LIST_HEAD);
	if(it == NULL) return RSGE_ERROR_MALLOC;
	while((node = list_iterator_next(it))) {
		rsge_input_device_t* tmp_dev = (rsge_input_device_t*)node->val;
		if(!strcmp(tmp_dev->name,name)) {
			*device = tmp_dev;
			list_iterator_destroy(it);
			return RSGE_ERROR_NONE;
		}
	}
	list_iterator_destroy(it);
	return RSGE_ERROR_INVALID_DEVICE;
}

rsge_error_e rsge_input_registerdev(rsge_input_device_t* device) {
	rsge_error_e err;
	rsge_input_device_t* tmp_dev;
	err = rsge_input_getdev_byname(&tmp_dev,device->name);
	if(err == RSGE_ERROR_INVALID_DEVICE) {
		device->callbacks->match = (int (*)(void*,void*))rsge_input_findcb;
		list_node_t* node = list_node_new(device);
		if(!node) return RSGE_ERROR_MALLOC;
		list_rpush(rsge_input_devices,node);
		return RSGE_ERROR_NONE;
	} else if(err != RSGE_ERROR_NONE) return err;
	return RSGE_ERROR_INVALID_DEVICE;
}

rsge_error_e rsge_input_registerdev_fromFile(char* path) {
	char* asset_data;
	size_t asset_size;
	rsge_error_e err = rsge_asset_read(path,&asset_data,&asset_size);
	if(err != RSGE_ERROR_NONE) {
		log_error("Failed to read asset");
		return err;
	}

	xmlDocPtr doc = xmlReadMemory(asset_data,asset_size,path,NULL,0);
	if(doc == NULL) {
		log_error("Cannot parse XML document");
		return RSGE_ERROR_LIBXML;
	}
	xmlNodePtr node = xmlDocGetRootElement(doc);
	if(node == NULL) {
		log_error("Cannot get root XML node");
		xmlFreeDoc(doc);
		return RSGE_ERROR_LIBXML;
	}

	if(!!xmlStrcmp(node->name,(const xmlChar*)"inputDevice")) {
		log_error("XML document root node is not the inputDevice tag");
		xmlFreeDoc(doc);
		return RSGE_ERROR_LIBXML;
	}

	rsge_input_device_t* device = malloc(sizeof(rsge_input_device_t));
	if(!device) {
		log_error("Failed to allocate %d bytes of memory",sizeof(rsge_input_device_t));
		xmlFreeDoc(doc);
		return RSGE_ERROR_MALLOC;
	}

	device->callbacks = list_new();
	if(!device->callbacks) {
		free(device);
		xmlFreeDoc(doc);
		return RSGE_ERROR_MALLOC;
	}

	char* device_type = (char*)xmlGetProp(node,"type");
	if(device_type == NULL) {
		log_error("inputDevice element is missing type property");
		xmlFreeDoc(doc);
		free(device);
		return RSGE_ERROR_LIBXML;
	}

	if(!strcmp(device_type,"joystick")) device->type = RSGE_INPUT_DEVICE_TYPE_JOYSTICK;
	else if(!strcmp(device_type,"mouse")) device->type = RSGE_INPUT_DEVICE_TYPE_MOUSE;
	else if(!strcmp(device_type,"keyboard")) device->type = RSGE_INPUT_DEVICE_TYPE_KEYBOARD;
	else if(!strcmp(device_type,"pathdrop")) {
		device->type = RSGE_INPUT_DEVICE_TYPE_PATHDROP;

		device->pathdrop.paths = NULL;
		device->pathdrop.pathCount = 0;
	} else return RSGE_ERROR_LIBXML;

	char* device_name = (char*)xmlGetProp(node,"name");
	if(device_name == NULL) {
		log_error("inputDevice element is missing name property");
		xmlFreeDoc(doc);
		free(device);
		return RSGE_ERROR_LIBXML;
	}
	device->name = device_name;

	xmlNodePtr cur = node->children;
	while(cur != NULL) {
		if(device->type == RSGE_INPUT_DEVICE_TYPE_JOYSTICK) {
			if(!xmlStrcmp(cur->name,(const xmlChar*)"axes")) {
				device->joystick.axisCount = 0;
				xmlNodePtr axisCur = cur->children;
				while(axisCur != NULL) {
					if(axisCur->type == XML_ELEMENT_NODE && !xmlStrcmp(axisCur->name,(const xmlChar*)"axis")) device->joystick.axisCount++;
					axisCur = axisCur->next;
				}

				device->joystick.axes = malloc(sizeof(rsge_input_device_axis_t)*device->joystick.axisCount);
				if(!device->joystick.axes) {
					log_error("Failed to allocate %d bytes of memory",sizeof(rsge_input_device_axis_t)*device->joystick.axisCount);
					xmlFreeDoc(doc);
					free(device);
					return RSGE_ERROR_MALLOC;
				}

				int i = 0;
				axisCur = cur->children;
				while(axisCur != NULL) {
					if(axisCur->type == XML_ELEMENT_NODE && !xmlStrcmp(axisCur->name,(const xmlChar*)"axis")) {
						rsge_input_device_axis_t* axis = (rsge_input_device_axis_t*)&device->joystick.axes[i];
						memset(axis,0,sizeof(rsge_input_device_axis_t));

						char* axisType = (char*)xmlGetProp(axisCur,"type");
						if(!axisType) {
							xmlFreeDoc(doc);
							free(device);
							return RSGE_ERROR_LIBXML;
						}

						if(!strcmp(axisType,"joystick")) axis->type = RSGE_INPUT_DEVICE_AXIS_TYPE_JOYSTICK;
						else if(!strcmp(axisType,"trigger")) axis->type = RSGE_INPUT_DEVICE_AXIS_TYPE_TRIGGER;
						else if(!strcmp(axisType,"dpad")) axis->type = RSGE_INPUT_DEVICE_AXIS_TYPE_DPAD;
						else return RSGE_ERROR_LIBXML;

						char* axisName = (char*)xmlGetProp(axisCur,"name");
						if(!axisName) {
							xmlFreeDoc(doc);
							free(device);
							return RSGE_ERROR_LIBXML;
						}
						axis->name = axisName;

						i++;
					}
					axisCur = axisCur->next;
				}
			}
			if(!xmlStrcmp(cur->name,(const xmlChar*)"buttons")) {
				device->joystick.buttonCount = 0;
				xmlNodePtr buttonCur = cur->children;
				while(buttonCur != NULL) {
					if(buttonCur->type == XML_ELEMENT_NODE && !xmlStrcmp(buttonCur->name,(const xmlChar*)"button")) device->joystick.buttonCount++;
					buttonCur = buttonCur->next;
				}

				device->joystick.buttons = malloc(sizeof(rsge_input_device_button_t)*device->joystick.buttonCount);
				if(!device->joystick.buttons) {
					log_error("Failed to allocate %d bytes of memory",sizeof(rsge_input_device_button_t)*device->joystick.buttonCount);
					xmlFreeDoc(doc);
					free(device);
					return RSGE_ERROR_MALLOC;
				}

				int i = 0;
				buttonCur = cur->children;
				while(buttonCur != NULL) {
					if(buttonCur->type == XML_ELEMENT_NODE && !xmlStrcmp(buttonCur->name,(const xmlChar*)"button")) {
						rsge_input_device_button_t* button = (rsge_input_device_button_t*)&device->joystick.buttons[i];
						memset(button,0,sizeof(rsge_input_device_button_t));

						char* buttonName = (char*)xmlGetProp(buttonCur,"name");
						if(!buttonName) {
							xmlFreeDoc(doc);
							free(device);
							return RSGE_ERROR_LIBXML;
						}

						button->name = buttonName;
						i++;
					}
					buttonCur = buttonCur->next;
				}
			}
		}
		cur = cur->next;
	}

	err = rsge_input_registerdev(device);
	xmlFreeDoc(doc);
	return err;
}

rsge_error_e rsge_input_emit(rsge_input_device_t* device) {
	device->callbacks->match = (int (*)(void*,void*))rsge_input_findcb;
	list_node_t* node;
	list_iterator_t* it = list_iterator_new(device->callbacks,LIST_HEAD);
	if(it == NULL) return RSGE_ERROR_MALLOC;
	while((node = list_iterator_next(it))) {
		rsge_input_event_t* event = (rsge_input_event_t*)node->val;
		if(event->cb != NULL) event->cb(device,event->userdata);
	}
	list_iterator_destroy(it);
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_input_addcb(char* devname,void (*cb)(rsge_input_device_t*,void*),void* userdata) {
	rsge_input_device_t* device;
	rsge_error_e err = rsge_input_getdev_byname(&device,devname);
	if(err != RSGE_ERROR_NONE) return err;
	device->callbacks->match = (int (*)(void*,void*))rsge_input_findcb;
	rsge_input_event_t* event = malloc(sizeof(rsge_input_event_t));
	if(!event) return RSGE_ERROR_MALLOC;
	event->cb = cb;
	event->userdata = userdata;
	list_node_t* node = list_node_new(event);
	if(!node) {
		free(event);
		return RSGE_ERROR_MALLOC;
	}
	list_rpush(device->callbacks,node);
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_input_rmcb(char* devname,void (*cb)(rsge_input_device_t*,void*)) {
	rsge_input_device_t* device;
	rsge_error_e err = rsge_input_getdev_byname(&device,devname);
	if(err != RSGE_ERROR_NONE) return err;
	device->callbacks->match = (int (*)(void*,void*))rsge_input_findcb;
	list_node_t* node = list_find(device->callbacks,cb);
	if(!node) return RSGE_ERROR_MALLOC;
	list_remove(device->callbacks,node);
	return RSGE_ERROR_NONE;
}