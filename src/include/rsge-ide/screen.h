#ifndef __RSGE_IDE_SCREEN_H_
#define __RSGE_IDE_SCREEN_H_ 1

#include <gtk/gtk.h>
#include <rsge-ide/error.h>

typedef struct rsge_ide_screen {
    void* impl;
} rsge_ide_screen_t;

#endif