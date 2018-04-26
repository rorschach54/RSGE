#ifndef __RSGE_IDE_SCREEN_H_
#define __RSGE_IDE_SCREEN_H_ 1

#include <gtk/gtk.h>
#include <rsge-ide/error.h>

typedef struct rsge_ide_screen {
} rsge_ide_screen_t;

#define RSGE_IDE_SCREEN_CREATE_PROTO(name) rsge_ide_error_e rsge_ide_screen_##name_create(rsge_ide_screen_t* screen)

#define RSGE_IDE_SCREEN_CREATE(name,screen) rsge_ide_screen_##name_create(screen)

#endif