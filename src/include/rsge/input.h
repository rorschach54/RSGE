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
    RSGE_INPUTMAP_INPUT_MOUSE
} rsge_inputmap_entry_type_e;

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

typedef void (*rsge_inputmap_entry_cb)(void* data);

typedef struct {
    rsge_inputmap_entry_type_e type;
    rsge_inputmap_entry_cb cb;
} rsge_inputmap_entry_t;

#endif