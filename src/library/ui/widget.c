#include <rsge/ui/widget.h>
#include <string.h>

rsge_error_e rsge_ui_widget_loadStyles(rsge_ui_widget_t* widget,rsge_ui_surface_t* ui,xmlDocPtr doc,xmlNodePtr node) {
    list_node_t* no;
    xmlNodePtr cur = node->xmlChildrenNode;
    while(cur != NULL) {
        char* value_raw = (char*)xmlGetProp(cur,"value");
        if(!value_raw) return RSGE_ERROR_LIBXML;
        
        void* value = value_raw;
        
        if(!xmlStrcmp(cur->name,(const xmlChar*)"backgroundColor")
            || !xmlStrcmp(cur->name,(const xmlChar*)"foregroundColor")
            || !xmlStrcmp(cur->name,(const xmlChar*)"textColor")) {
            if(value_raw[0] == '#') {
                value_raw++;
                int color[4];
                color[0] = color[1] = color[2] = 0;
                color[3] = 0xFF;
                if(strlen(value_raw) == 2) color[0] = (int)strtol(value_raw,NULL,16);
                else if(strlen(value_raw) == 4) {
                    char raw_red[2];
                    char raw_green[2];
                    
                    memcpy(raw_red,value_raw,2);
                    memcpy(raw_green,value_raw+2,2);
                    
                    color[0] = (int)strtol(raw_red,NULL,16);
                    color[1] = (int)strtol(raw_green,NULL,16);
                } else if(strlen(value_raw) == 6) {
                    char raw_red[2];
                    char raw_green[2];
                    char raw_blue[2];
                    
                    memcpy(raw_red,value_raw,2);
                    memcpy(raw_green,value_raw+2,2);
                    memcpy(raw_blue,value_raw+4,2);
                    
                    color[0] = (int)strtol(raw_red,NULL,16);
                    color[1] = (int)strtol(raw_green,NULL,16);
                    color[2] = (int)strtol(raw_blue,NULL,16);
                } else if(strlen(value_raw) == 8) {
                    char raw_red[2];
                    char raw_green[2];
                    char raw_blue[2];
                    char raw_alpha[2];
                    
                    memcpy(raw_red,value_raw,2);
                    memcpy(raw_green,value_raw+2,2);
                    memcpy(raw_blue,value_raw+4,2);
                    memcpy(raw_alpha,value_raw+6,2);
                    
                    color[0] = (int)strtol(raw_red,NULL,16);
                    color[1] = (int)strtol(raw_green,NULL,16);
                    color[2] = (int)strtol(raw_blue,NULL,16);
                    color[3] = (int)strtol(raw_alpha,NULL,16);
                }
                
                value = color;
            }
        }
        
        rsge_ui_widget_style_t style = {
            .name = (char*)cur->name,
            .value = value
        };
        
        void* tmp;
        rsge_error_e err = rsge_ui_widget_getstyle(widget,style.name,&tmp);
        if(err) {
            no = list_node_new(&style);
            if(!no) return RSGE_ERROR_MALLOC;
            list_rpush(widget->styles,no);
        } else {
            list_iterator_t* it = list_iterator_new(widget->styles,LIST_HEAD);
            if(it == NULL) return RSGE_ERROR_MALLOC;
            while((no = list_iterator_next(it))) {
                if(!strcmp(((rsge_ui_widget_style_t*)no->val)->name,style.name)) list_remove(widget->styles,no);
            }
            list_iterator_destroy(it);
            
            no = list_node_new(&style);
            if(!no) return RSGE_ERROR_MALLOC;
            list_rpush(widget->styles,no);
        }
        cur = cur->next;
    }
    return RSGE_ERROR_NONE;
}

rsge_error_e rsge_ui_widget_loadEvents(rsge_ui_widget_t* widget,rsge_ui_surface_t* ui,xmlDocPtr doc,xmlNodePtr node) {
    list_node_t* no;
    xmlNodePtr cur = node->xmlChildrenNode;
    while(cur != NULL) {
        cur = cur->next;
    }
    return RSGE_ERROR_NONE;
}

rsge_error_e rsge_ui_widget_create(rsge_ui_widget_t* widget,rsge_ui_surface_t* ui,xmlDocPtr doc,xmlNodePtr node) {
    memset(widget,0,sizeof(rsge_ui_widget_t));
    
    widget->baseXML.doc = doc;
    widget->baseXML.node = node;
    
    widget->styles = list_new();
    if(!widget->styles) return RSGE_ERROR_MALLOC;
    
    widget->events = list_new();
    if(!widget->events) {
        list_destroy(widget->styles);
        return RSGE_ERROR_MALLOC;
    }
    
    rsge_error_e err;
    xmlNodePtr cur = node->xmlChildrenNode;
    while(cur != NULL) {
        if(!xmlStrcmp(cur->name,(const xmlChar*)"styles")) {
            err = rsge_ui_widget_loadStyles(widget,ui,doc,cur);
            if(err != RSGE_ERROR_NONE) {
                list_destroy(widget->styles);
                list_destroy(widget->events);
                return err;
            }
        }
        if(!xmlStrcmp(cur->name,(const xmlChar*)"events")) {
            err = rsge_ui_widget_loadEvents(widget,ui,doc,cur);
            if(err != RSGE_ERROR_NONE) {
                list_destroy(widget->styles);
                list_destroy(widget->events);
                return err;
            }
        }
        cur = cur->next;
    }
    return RSGE_ERROR_NONE;
}

rsge_error_e rsge_ui_widget_getstyle(rsge_ui_widget_t* widget,char* name,void** value) {
    list_node_t* node;
    list_iterator_t* it = list_iterator_new(widget->styles,LIST_HEAD);
    if(it == NULL) return RSGE_ERROR_MALLOC;
    while((node = list_iterator_next(it))) {
        rsge_ui_widget_style_t* style = (rsge_ui_widget_style_t*)node->val;
        if(!strcmp(style->name,name)) {
            *value = style->value;
            list_iterator_destroy(it);
            return RSGE_ERROR_NONE;
        }
    }
    list_iterator_destroy(it);
    return RSGE_ERROR_INVALID_STYLE;
}