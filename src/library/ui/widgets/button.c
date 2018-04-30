#include <rsge/assets/rsge_assets.h>
#include <rsge/gfx/font.h>
#include <rsge/ui/widgets/button.h>
#include <rsge/assets.h>
#include <rsge/input.h>
#include <string.h>

typedef struct {
    char* text;
    
    rsge_font_t font;
    
    rsge_ui_surface_t* ui;
} rsge_ui_widget_button_t;

static void rsge_ui_widget_button_mouse_callback(rsge_input_device_t* device,void* userdata) {
    rsge_ui_widget_t* widget = (rsge_ui_widget_t*)userdata;
    rsge_ui_widget_button_t* button = (rsge_ui_widget_button_t*)widget->impl;
    rsge_error_e err;
    
    /* Load the default style */
    xmlNodePtr cur = widget->baseXML.node->children;
    while(cur != NULL) {
        if(!xmlStrcmp(cur->name,(const xmlChar*)"styles")) {
            err = rsge_ui_widget_loadStyles(widget,button->ui,widget->baseXML.doc,cur->children);
            if(err != RSGE_ERROR_NONE) continue;
            break;
        }
        cur = cur->next;
    }
    
    list_node_t* node;
    list_iterator_t* it = list_iterator_new(widget->events,LIST_HEAD);
    if(it == NULL) return;
    while((node = list_iterator_next(it))) {
        rsge_ui_widget_event_t* event = (rsge_ui_widget_event_t*)node->val;
        /* Check mouse X position */
        if(widget->x-device->mouse.posX >= 0 && widget->x-device->mouse.posX < widget->width) {
            /* Check mouse Y position */
            if(widget->y-device->mouse.posY >= 0 && widget->y-device->mouse.posX < widget->height) {
                /* Find which button was pressed */
                if(device->mouse.buttonLeft && !strcmp(event->type,"mouse.click")) {
                    err = rsge_ui_widget_loadStyles(widget,button->ui,widget->baseXML.doc,event->stylesNode);
                    if(err != RSGE_ERROR_NONE) continue;
                    
                    // TODO: call UI event callbacks (onclick, onactive)
                } else if(!device->mouse.buttonLeft && !device->mouse.buttonMiddle && !device->mouse.buttonRight && !strcmp(event->type,"mouse.hover")) {
                    err = rsge_ui_widget_loadStyles(widget,button->ui,widget->baseXML.doc,event->stylesNode);
                    if(err != RSGE_ERROR_NONE) continue;
                }
            }
        }
    }
    list_iterator_destroy(it);
}

rsge_error_e rsge_ui_widget_button_destroy(rsge_ui_widget_t* widget) {
    rsge_ui_widget_button_t* button = (rsge_ui_widget_button_t*)widget->impl;
    
    rsge_font_destroy(&button->font);
    rsge_input_rmcb("mouse",rsge_ui_widget_button_mouse_callback);
    
    free(button);
    memset(widget,0,sizeof(rsge_ui_widget_t));
    return RSGE_ERROR_NONE;
}

rsge_error_e rsge_ui_widget_button_update(rsge_ui_widget_t* widget,rsge_ui_surface_t* ui) {
    rsge_ui_widget_button_t* button = (rsge_ui_widget_button_t*)widget->impl;
    rsge_error_e err;
    
    int textColor[4];
    err = rsge_ui_widget_getstyle(widget,"textColor",(void**)&textColor);
    if(err != RSGE_ERROR_NONE) return err;
    
    rsge_surface_t textSurface;
    err = rsge_font_render(&button->font,&textSurface,button->text,textColor);
    if(err != RSGE_ERROR_NONE) return err;
    
    int backgroundColor[4];
    err = rsge_ui_widget_getstyle(widget,"backgroundColor",(void**)&backgroundColor);
    if(err != RSGE_ERROR_NONE) {
        for(size_t y = 0;y < textSurface.height;y++) {
            for(size_t x = 0;x < textSurface.width;x++) {
                size_t off = textSurface.bpp*(x+y*textSurface.width);
                size_t soff = ui->surface.bpp*((widget->x+x)+(widget->y+y)*ui->surface.width);
                if(textSurface.buffer[off] == textColor[0]
                    && textSurface.buffer[off+1] == textColor[1]
                    && textSurface.buffer[off+2] == textColor[2]
                    && textSurface.buffer[off+3] == textColor[3]) {
                    ui->surface.buffer[soff] = textColor[0];
                    ui->surface.buffer[soff+1] = textColor[1];
                    ui->surface.buffer[soff+2] = textColor[2];
                    ui->surface.buffer[soff+3] = textColor[3];
                }
            }
        }
    } else {
        for(size_t y = 0;y < textSurface.height;y++) {
            for(size_t x = 0;x < textSurface.width;x++) {
                size_t off = textSurface.bpp*(x+y*textSurface.width);
                if(textSurface.buffer[off] != textColor[0]
                    && textSurface.buffer[off+1] != textColor[1]
                    && textSurface.buffer[off+2] != textColor[2]
                    && textSurface.buffer[off+3] != textColor[3]) {
                    textSurface.buffer[off] = backgroundColor[0];
                    textSurface.buffer[off+1] = backgroundColor[1];
                    textSurface.buffer[off+2] = backgroundColor[2];
                    textSurface.buffer[off+3] = backgroundColor[3];
                }
            }
        }
        vec2 pos;
        pos[0] = widget->x;
        pos[1] = widget->y;
        err = rsge_surface_blit(&ui->surface,&textSurface,pos);
        if(err != RSGE_ERROR_NONE) {
            rsge_surface_destroy(&textSurface);
            return err;
        }
    }
    rsge_surface_destroy(&textSurface);
    return RSGE_ERROR_NONE;
}

rsge_error_e rsge_ui_widget_button_create(rsge_ui_widget_t* widget,rsge_ui_surface_t* ui,char* text) {
    rsge_error_e err;
    rsge_asset_t baseWidgetAsset;
    err = rsge_asset_get(&baseWidgetAsset,"rsge@ui/widgets/button.xml");
    if(err != RSGE_ERROR_NONE) return err;
    xmlDocPtr doc = xmlReadMemory((char*)baseWidgetAsset.data,baseWidgetAsset.size,baseWidgetAsset.name,NULL,0);
    if(doc == NULL) return RSGE_ERROR_LIBXML;
    xmlNodePtr node = xmlDocGetRootElement(doc);
    if(node == NULL) {
        xmlFreeDoc(doc);
        return RSGE_ERROR_LIBXML;
    }
    err = rsge_ui_widget_create(widget,ui,doc,node);
    if(err != RSGE_ERROR_NONE) return err;
    
    rsge_ui_widget_button_t* button = malloc(sizeof(rsge_ui_widget_button_t));
    if(!button) return RSGE_ERROR_MALLOC;
    button->ui = ui;
    
    button->text = text;
    widget->destroy = rsge_ui_widget_button_destroy;
    widget->update = rsge_ui_widget_button_update;
    
    widget->width = ui->surface.width/10;
    widget->height = ui->surface.height/10;
    
    char* fontPath;
    err = rsge_ui_widget_getstyle(widget,"font",(void**)&fontPath);
    if(err != RSGE_ERROR_NONE) return err;
    
    err = rsge_font_fromFile(&button->font,fontPath,widget->height/3);
    if(err != RSGE_ERROR_NONE) {
        free(button);
        return err;
    }
    
    widget->impl = (void*)button;
    
    err = rsge_input_addcb("mouse",rsge_ui_widget_button_mouse_callback,widget);
    if(err != RSGE_ERROR_NONE) {
        free(button);
        return err;
    }
    return RSGE_ERROR_NONE;
}

rsge_error_e rsge_ui_widget_button_fromXMLNode(rsge_ui_widget_t* widget,rsge_ui_surface_t* ui,xmlDocPtr doc,xmlNodePtr node) {
    rsge_error_e err = rsge_ui_widget_button_create(widget,ui,(char*)xmlNodeListGetString(doc,node,1));
    if(err != RSGE_ERROR_NONE) return err;
    
    widget->instXML.doc = doc;
    widget->instXML.node = node;
    
    return RSGE_ERROR_NONE;
}