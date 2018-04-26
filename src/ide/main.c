#include <gtk/gtk.h>
#include <rsge-ide/screens/startup.h>
#include <stdlib.h>

rsge_ide_screen_t rsge_ide_screen_startup;

static void activate(GtkApplication* app,gpointer user_data) {
    /* Initialize settings */
    GSettings* settings = g_settings_new("com.rosssoftware.rsge.ide");

    /* Create window */
    GtkWidget* window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window),"RSGE IDE");
    gtk_window_set_default_size(GTK_WINDOW(window),800,600);
    
    /* Initialize screens */
    rsge_ide_error_e err;
    
    err = RSGE_IDE_SCREEN_CREATE(startup,&rsge_ide_screen_startup);
    if(err != RSGE_IDE_ERROR_NONE) return;
    
    // TODO: show startup screen
    
    /* Show window */
    gtk_widget_show_all(window);
}

int main(int argc,char** argv) {
    GtkApplication* app;
    int status = EXIT_SUCCESS;
    app = gtk_application_new("com.rosssoftware.rsge.ide",G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app,"activate",G_CALLBACK(activate),NULL);
    status = g_application_run(G_APPLICATION(app),argc,argv);
    g_object_unref(app);
    return status;
}