#ifndef __RSGE_INPUT_H_
#define __RSGE_INPUT_H_ 1

#include <rsge/error.h>

typedef enum {
    RSGE_INPUTMAP_NONE,
    RSGE_INPUTMAP_RELEASE,
    RSGE_INPUTMAP_PRESS,
    RSGE_INPUTMAP_REPEAT
} rsge_inputmap_entry_action_e;

typedef enum {
    RSGE_INPUTMAP_INPUT_KB,
    RSGE_INPUTMAP_INPUT_MOUSE,
    RSGE_INPUTMAP_INPUT_JOYSTICK
} rsge_inputmap_entry_type_e;

typedef enum {
    RSGE_INPUTMAP_JOYSTICK_EVENT_CONN,
    RSGE_INPUTMAP_JOYSTICK_EVENT_DISCONN,
    RSGE_INPUTMAP_JOYSTICK_EVENT_AXIS_BUTTON
} rsge_inputmap_entry_joystick_event_e;

typedef struct {
    char* key;
    rsge_inputmap_entry_action_e action;
} rsge_inputmap_key_t;

typedef struct {
    double x;
    double y;
    int button;
    rsge_inputmap_entry_action_e action;
} rsge_inputmap_mouse_t;

typedef struct {
    char* name;
    int id;
    
    int axisCount;
    float* axis;
    
    unsigned int* button;
    int buttonCount;
    
    rsge_inputmap_entry_joystick_event_e event;
} rsge_inputmap_joystick_t;

typedef void (*rsge_inputmap_entry_cb)(void* data);

typedef struct {
    rsge_inputmap_entry_type_e type;
    rsge_inputmap_entry_cb cb;
} rsge_inputmap_entry_t;

#endif