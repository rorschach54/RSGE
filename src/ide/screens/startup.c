#include <rsge-ide/screens/startup.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    GtkWidget* container;
    GtkWidget* startupMessage;
} rsge_ide_screen_startup_t;

rsge_ide_error_e rsge_ide_screen_startup_create(rsge_ide_screen_t* screen) {
	memset(screen,0,sizeof(rsge_ide_screen_t));

	rsge_ide_screen_startup_t* startup = malloc(sizeof(rsge_ide_screen_startup_t));
	if(!startup) return RSGE_IDE_ERROR_MALLOC;

	startup->container = gtk_fixed_new();

	startup->startupMessage = gtk_label_new("RSGE IDE is starting up");
	gtk_fixed_put(GTK_FIXED(startup->container),startup->startupMessage,0,0);

	screen->impl = (rsge_ide_screen_startup_t*)startup;
	return RSGE_IDE_ERROR_NONE;
}

rsge_ide_error_e rsge_ide_screen_startup_destroy(rsge_ide_screen_t* screen) {
	rsge_ide_screen_startup_t* startup = (rsge_ide_screen_startup_t*)screen->impl;
	free(startup);
	memset(screen,0,sizeof(rsge_ide_screen_t));
	return RSGE_IDE_ERROR_NONE;
}

rsge_ide_error_e rsge_ide_screen_startup_show(rsge_ide_screen_t* screen,GtkWidget* window) {
	rsge_ide_screen_startup_t* startup = (rsge_ide_screen_startup_t*)screen->impl;
	gtk_window_set_default_size(GTK_WINDOW(window),300,300);

	int res_w;
	int res_h;
	gtk_window_get_size(GTK_WINDOW(window),&res_w,&res_h);

	gtk_fixed_move(GTK_FIXED(startup->container),startup->startupMessage,res_w/2,res_h/2);

	gtk_container_add(GTK_CONTAINER(window),startup->container);
	return RSGE_IDE_ERROR_NONE;
}

rsge_ide_error_e rsge_ide_screen_startup_hide(rsge_ide_screen_t* screen,GtkWidget* window) {
	rsge_ide_screen_startup_t* startup = (rsge_ide_screen_startup_t*)screen->impl;
	gtk_window_set_default_size(GTK_WINDOW(window),800,600);
	return RSGE_IDE_ERROR_NONE;
}
