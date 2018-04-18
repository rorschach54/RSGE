#include <rsge/ui/xml/widget.h>
#include <rsge/ui.h>
#include <list.h>

rsge_error_e rsge_ui_widget_fromXMLNode(rsge_ui_widget_t* widget,rsge_ui_surface_t* ui,xmlDocPtr doc,xmlNodePtr node) {
    rsge_ui_widget_type_t* type;
    rsge_error_e err = rsge_ui_getregisteredwidget((char*)node->name,&type);
    if(err != RSGE_ERROR_NONE) return err;
    
    if(type->fromXMLNode != NULL) return type->fromXMLNode(widget,ui,doc,node);
    return RSGE_ERROR_INVALID_WIDGET;
}

rsge_error_e rsge_ui_widget_fromAssets(rsge_ui_widget_t* widget,rsge_ui_surface_t* ui,rsge_asset_t* asset) {
    xmlDocPtr doc = xmlReadMemory((char*)asset->data,asset->size,asset->name,NULL,0);
    if(doc == NULL) return RSGE_ERROR_LIBXML;
    xmlNodePtr node = xmlDocGetRootElement(doc);
    if(node == NULL) {
        xmlFreeDoc(doc);
        return RSGE_ERROR_LIBXML;
    }
    return rsge_ui_widget_fromXMLNode(widget,ui,doc,node);
}