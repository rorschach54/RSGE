#include <rsge/ui/surface.h>
#include <rsge/ui/widget.h>
#include <string.h>

rsge_error_e rsge_ui_surface_create(rsge_ui_surface_t* ui,size_t width,size_t height) {
    memset(ui,0,sizeof(rsge_ui_surface_t));
    
    rsge_error_e err = rsge_obj_texture_create(&ui->texture);
    if(err != RSGE_ERROR_NONE) return err;
    
    ui->texture.width = width;
    ui->texture.height = height;
    ui->texture.pixels = malloc(((ui->texture.width+4-(ui->texture.width % 4))*ui->texture.height));
    if(!ui->texture.pixels) return RSGE_ERROR_MALLOC;
    
    ui->widgets = list_new();
    if(!ui->widgets) {
        return RSGE_ERROR_MALLOC;
    }
    return RSGE_ERROR_NONE;
}

rsge_error_e rsge_ui_surface_destroy(rsge_ui_surface_t* ui) {
    list_node_t* node;
    list_iterator_t* it = list_iterator_new(ui->widgets,LIST_HEAD);
    if(!it) return RSGE_ERROR_MALLOC;
    while((node = list_iterator_next(it))) {
        rsge_ui_widget_t* widget = (rsge_ui_widget_t*)node->val;
        if(widget->destroy != NULL) {
            rsge_error_e err = widget->destroy(widget);
            if(err != RSGE_ERROR_NONE) return err;
        }
    }
    list_iterator_destroy(it);
    free(ui->texture.pixels);
    list_destroy(ui->widgets);
    memset(ui,0,sizeof(rsge_ui_surface_t));
    return RSGE_ERROR_NONE;
}

rsge_error_e rsge_ui_surface_update(rsge_ui_surface_t* ui) {
    list_node_t* node;
    list_iterator_t* it = list_iterator_new(ui->widgets,LIST_HEAD);
    if(!it) return RSGE_ERROR_MALLOC;
    while((node = list_iterator_next(it))) {
        rsge_ui_widget_t* widget = (rsge_ui_widget_t*)node->val;
        if(widget->update != NULL) {
            rsge_error_e err = widget->update(widget,ui);
            if(err != RSGE_ERROR_NONE) return err;
        }
    }
    list_iterator_destroy(it);
    return RSGE_ERROR_NONE;
}