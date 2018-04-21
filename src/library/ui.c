#include <rsge/ui/widgets/button.h>
#include <rsge/ui/widgets/label.h>
#include <rsge/ui.h>
#include <list.h>

list_t* rsge_ui_widget_types;

rsge_error_e rsge_ui_init() {
    rsge_ui_widget_types = list_new();
    if(!rsge_ui_widget_types) return RSGE_ERROR_MALLOC;
    
    rsge_error_e err;
    
    /* Register widgets */
    
    /* Register widget: button (rsge.widget.button) */
    rsge_ui_widget_type_t rsge_widget_button = {
        .tag = "rsge.widget.button",
        .fromXMLNode = rsge_ui_widget_button_fromXMLNode
    };
    err = rsge_ui_registerWidget(&rsge_widget_button);
    if(err != RSGE_ERROR_NONE) return err;
    
    /* Register widget: label (rsge.widget.label) */
    rsge_ui_widget_type_t rsge_widget_label = {
        .tag = "rsge.widget.label",
        .fromXMLNode = rsge_ui_widget_label_fromXMLNode
    };
    err = rsge_ui_registerWidget(&rsge_widget_label);
    if(err != RSGE_ERROR_NONE) return err;
    
    return RSGE_ERROR_NONE;
}

rsge_error_e rsge_ui_deinit() {
    list_destroy(rsge_ui_widget_types);
    return RSGE_ERROR_NONE;
}

rsge_error_e rsge_ui_registerWidget(rsge_ui_widget_type_t* type) {
    rsge_ui_widget_type_t* tmp_type;
    rsge_error_e err = rsge_ui_getregisteredwidget(type->tag,&tmp_type);
    if(err == RSGE_ERROR_INVALID_WIDGET) {
        list_node_t* node = list_node_new(type);
        if(!node) return RSGE_ERROR_MALLOC;
        list_rpush(rsge_ui_widget_types,node);
        return RSGE_ERROR_NONE;
    } else if(err != RSGE_ERROR_NONE) return err;
    return RSGE_ERROR_INVALID_WIDGET;
}

rsge_error_e rsge_ui_getregisteredwidget(char* tag,rsge_ui_widget_type_t** type) {
    list_node_t* node;
    list_iterator_t* it = list_iterator_new(rsge_ui_widget_types,LIST_HEAD);
    if(it == NULL) return RSGE_ERROR_MALLOC;
    while((node = list_iterator_next(it))) {
        rsge_ui_widget_type_t* tmp_type = (rsge_ui_widget_type_t*)node->val;
        if(!strcmp(tmp_type->tag,tag)) {
            *type = tmp_type;
            list_iterator_destroy(it);
            return RSGE_ERROR_NONE;
        }
    }
    list_iterator_destroy(it);
    return RSGE_ERROR_INVALID_WIDGET;
}
