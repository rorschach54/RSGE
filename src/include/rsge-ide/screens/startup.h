#ifndef __RSGE_IDE_SCREENS_STARTUP_H_
#define __RSGE_IDE_SCREENS_STARTUP_H_ 1

#include <gtk/gtk.h>
#include <rsge-ide/screen.h>

rsge_ide_error_e rsge_ide_screen_startup_create(rsge_ide_screen_t* screen);
rsge_ide_error_e rsge_ide_screen_startup_destroy(rsge_ide_screen_t* screen);
rsge_ide_error_e rsge_ide_screen_startup_show(rsge_ide_screen_t* screen,GtkWidget* window);
rsge_ide_error_e rsge_ide_screen_startup_hide(rsge_ide_screen_t* screen,GtkWidget* window);

#endif