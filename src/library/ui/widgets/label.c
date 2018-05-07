#include <rsge/assets/rsge_assets.h>
#include <rsge/gfx/font.h>
#include <rsge/ui/widgets/label.h>
#include <rsge/assets.h>
#include <string.h>

#ifndef DOXYGEN_SHOULD_SKIP_THIS
typedef struct {
    char* text;
    
    rsge_font_t font;
} rsge_ui_widget_label_t;

static rsge_error_e rsge_ui_widget_label_destroy(rsge_ui_widget_t* widget) {
    rsge_ui_widget_label_t* label = (rsge_ui_widget_label_t*)widget->impl;
    
    rsge_font_destroy(&label->font);
    
    free(label);
    memset(widget,0,sizeof(rsge_ui_widget_t));
    return RSGE_ERROR_NONE;
}

static rsge_error_e rsge_ui_widget_label_update(rsge_ui_widget_t* widget,rsge_ui_surface_t* ui) {
    rsge_ui_widget_label_t* label = (rsge_ui_widget_label_t*)widget->impl;
    rsge_error_e err;
    
    int textColor[4];
    err = rsge_ui_widget_getstyle(widget,"textColor",(void**)&textColor);
    if(err != RSGE_ERROR_NONE) return err;
    
    rsge_obj_texture_t textTexture;
    err = rsge_font_render(&label->font,&textTexture,label->text,textColor);
    if(err != RSGE_ERROR_NONE) return err;
    
    int backgroundColor[4];
    err = rsge_ui_widget_getstyle(widget,"backgroundColor",(void**)&backgroundColor);
    if(err != RSGE_ERROR_NONE) {
        for(size_t y = 0;y < textTexture.height;y++) {
            for(size_t x = 0;x < textTexture.width;x++) {
                size_t off = (y*textTexture.width+x);
                size_t soff = ((widget->y+y)*ui->texture.width+(widget->x+x));
                if(textTexture.pixels[off].red == (float)(textColor[0]/255)
                    && textTexture.pixels[off].green == (float)(textColor[1]/255)
                    && textTexture.pixels[off].blue == (float)(textColor[2]/255)
                    && textTexture.pixels[off].alpha == (float)(textColor[3]/255)) {
                    ui->texture.pixels[soff].red = (float)(textColor[0]/255);
                    ui->texture.pixels[soff].green = (float)(textColor[1]/255);
                    ui->texture.pixels[soff].blue = (float)(textColor[2]/255);
                    ui->texture.pixels[soff].alpha = (float)(textColor[3]/255);
                }
            }
        }
    } else {
        for(size_t y = 0;y < textTexture.height;y++) {
            for(size_t x = 0;x < textTexture.width;x++) {
                size_t off = (y*textTexture.width+x);
                if(textTexture.pixels[off].red != (float)(textColor[0]/255)
                    && textTexture.pixels[off].green != (float)(textColor[1]/255)
                    && textTexture.pixels[off].blue != (float)(textColor[2]/255)
                    && textTexture.pixels[off].alpha != (float)(textColor[3]/255)) {
                    textTexture.pixels[off].red = (float)(backgroundColor[0]/255);
                    textTexture.pixels[off].green = (float)(backgroundColor[1]/255);
                    textTexture.pixels[off].blue = (float)(backgroundColor[2]/255);
                    textTexture.pixels[off].alpha = (float)(backgroundColor[3]/255);
                }
            }
        }
        // TODO: blit the texture.
    }
    free(textTexture.pixels);
    return RSGE_ERROR_NONE;
}

rsge_error_e rsge_ui_widget_label_create(rsge_ui_widget_t* widget,rsge_ui_surface_t* ui,char* text) {
    rsge_error_e err;
    rsge_asset_t baseWidgetAsset;
    err = rsge_asset_get(&baseWidgetAsset,"rsge@ui/widgets/label.xml");
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
    
    widget->width = ui->texture.width/10;
    widget->height = ui->texture.height/10;
    
    char* fontPath;
    err = rsge_ui_widget_getstyle(widget,"font",(void**)&fontPath);
    if(err != RSGE_ERROR_NONE) return err;
    
    err = rsge_font_fromFile(&label->font,fontPath,widget->height/3);
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
    
    widget->instXML.doc = doc;
    widget->instXML.node = node;
    
    return RSGE_ERROR_NONE;
}
#endif
