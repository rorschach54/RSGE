#include <rsge/ui/xml/widget.h>
#include <list.h>

extern list_t* rsge_ui_widget_types;

rsge_error_e rsge_ui_widget_fromXMLNode(rsge_ui_widget_t* widget,rsge_ui_surface_t* ui,xmlDocPtr doc,xmlNodePtr node) {
    if(!xmlStrcmp(node->name,(const xmlChar*)"rsge.widget.label")) {
        return RSGE_ERROR_NONE;
    }
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