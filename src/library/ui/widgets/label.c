#include <rsge/assets/rsge_assets.h>
#include <rsge/gfx/font.h>
#include <rsge/ui/widgets/label.h>
#include <rsge/assets.h>
#include <string.h>

typedef struct {
    char* text;
    
    rsge_font_t font;
    rsge_asset_t fontAsset;
} rsge_ui_widget_label_t;

rsge_error_e rsge_ui_widget_label_destroy(rsge_ui_widget_t* widget) {
    rsge_ui_widget_label_t* label = (rsge_ui_widget_label_t*)widget->impl;
    
    rsge_font_destroy(&label->font);
    
    free(label);
    memset(widget,0,sizeof(rsge_ui_widget_t));
    return RSGE_ERROR_NONE;
}

rsge_error_e rsge_ui_widget_label_update(rsge_ui_widget_t* widget,rsge_ui_surface_t* ui) {
    rsge_ui_widget_label_t* label = (rsge_ui_widget_label_t*)widget->impl;
    rsge_error_e err;
    
    int textColor[4];
    err = rsge_ui_widget_getstyle(widget,"textColor",(void**)&textColor);
    if(err != RSGE_ERROR_NONE) return err;
    
    int backgroundColor[4];
    err = rsge_ui_widget_getstyle(widget,"backgroundColor",(void**)&backgroundColor);
    if(err != RSGE_ERROR_NONE) return err;
    
    rsge_surface_t textSurface;
    err = rsge_font_render(&label->font,&textSurface,label->text,textColor);
    if(err != RSGE_ERROR_NONE) return err;
    
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
    return RSGE_ERROR_NONE;
}

rsge_error_e rsge_ui_widget_label_create(rsge_ui_widget_t* widget,rsge_ui_surface_t* ui,char* text) {
    rsge_error_e err;
    rsge_asset_t baseWidgetAsset;
    err = rsge_asset_find(&rsge_assets,&baseWidgetAsset,"ui/widgets/label.xml");
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
    
    rsge_ui_widget_label_t* label = malloc(sizeof(rsge_ui_widget_label_t));
    if(!label) return RSGE_ERROR_MALLOC;
    
    label->text = text;
    widget->destroy = rsge_ui_widget_label_destroy;
    widget->update = rsge_ui_widget_label_update;
    
    widget->width = ui->surface.width/10;
    widget->height = ui->surface.height/10;
    
    char* fontPath;
    err = rsge_ui_widget_getstyle(widget,"font",(void**)&fontPath);
    if(err != RSGE_ERROR_NONE) return err;
    
    err = rsge_asset_find(&rsge_assets,&label->fontAsset,fontPath);
    if(err != RSGE_ERROR_NONE) {
        free(label);
        return err;
    }
    
    err = rsge_font_fromFile(&label->font,&label->fontAsset,widget->height/3);
    if(err != RSGE_ERROR_NONE) {
        free(label);
        return err;
    }
    
    widget->impl = (void*)label;
    return RSGE_ERROR_NONE;
}

rsge_error_e rsge_ui_widget_label_fromXMLNode(rsge_ui_widget_t* widget,rsge_ui_surface_t* ui,xmlDocPtr doc,xmlNodePtr node) {
    rsge_error_e err = rsge_ui_widget_label_create(widget,ui,(char*)xmlNodeListGetString(doc,node,1));
    if(err != RSGE_ERROR_NONE) return err;
    return RSGE_ERROR_NONE;
}