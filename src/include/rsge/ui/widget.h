#ifndef __RSGE_UI_WIDGET_H_
#define __RSGE_UI_WIDGET_H_ 1

#include <rsge/gfx/colors.h>
#include <rsge/gfx/font.h>
#include <rsge/ui/surface.h>
#include <libxml/parser.h>
#include <list.h>
#include <string.h>

typedef struct {
    char* name;
    void* value;
} rsge_ui_widget_style_t;

typedef struct {
    /**
     * \brief Widget styling.
     */
    list_t* styles;
    
    /**
     * \brief The type of the event.
     */
    char* type;
} rsge_ui_widget_event_t;

typedef struct rsge_ui_widget {
    /**
     * \brief Widget styling.
     */
    list_t* styles;
    
    /**
     * \brief Widget events.
     */
    list_t* events;
    
    /**
     * \brief The width of the widget.
     */
    size_t width;
    
    /**
     * \brief The height of the widget.
     */
    size_t height;
    /**
     * \brief The X axis position of the widget.
     */
    size_t x;
    
    /**
     * \brief The Y axis position of the widget.
     */
    size_t y;
    
    /**
     * \brief Widget implementation data.
     */
    void* impl;
    
    /**
     * \brief Widget implemented destroy function.
     */
    rsge_error_e (*destroy)(struct rsge_ui_widget* widget);
    
    /**
     * \brief Widget implemented update function.
     */
    rsge_error_e (*update)(struct rsge_ui_widget* widget,rsge_ui_surface_t* ui);
    
    /**
     * \brief Base widget XML substructure.
     */
    struct {
        /**
         * \brief XML document.
         */
        xmlDocPtr doc;
        
        /**
         * \brief XML root node.
         */
        xmlNodePtr node;
    } baseXML;
    
    /**
     * \brief Instance widget XML substructure.
     */
    struct {
        /**
         * \brief XML document.
         */
        xmlDocPtr doc;
        
        /**
         * \brief XML root node.
         */
        xmlNodePtr node;
    } instXML;
} rsge_ui_widget_t;

typedef struct {
    /**
     * \brief XML tag name.
     */
    char* tag;
    
    rsge_error_e (*fromXMLNode)(rsge_ui_widget_t* widget,rsge_ui_surface_t* ui,xmlDocPtr doc,xmlNodePtr node);
} rsge_ui_widget_type_t;

/**
 * \fn rsge_error_e rsge_ui_widget_loadStyles(rsge_ui_widget_t* widget,rsge_ui_surface_t* ui,xmlDocPtr doc,xmlNodePtr node)
 * \brief Load the styles from an XML node.
 * \param widget The pointer to the widget.
 * \param ui The UI surface to use.
 * \param doc The XML document to use for base widget.
 * \param node The XML node to use.
 * \return An error code.
 */
rsge_error_e rsge_ui_widget_loadStyles(rsge_ui_widget_t* widget,rsge_ui_surface_t* ui,xmlDocPtr doc,xmlNodePtr node);

/**
 * \fn rsge_error_e rsge_ui_widget_create(rsge_ui_widget_t* widget,rsge_ui_surface_t* ui,xmlDocPtr doc,xmlNodePtr node)
 * \brief Creates a generic widget.
 * \param widget The pointer to the widget.
 * \param ui The UI surface to use.
 * \param doc The XML document to use for base widget.
 * \param node The XML node to use.
 * \return An error code.
 */
rsge_error_e rsge_ui_widget_create(rsge_ui_widget_t* widget,rsge_ui_surface_t* ui,xmlDocPtr doc,xmlNodePtr node);

/**
 * \fn rsge_error_e rsge_ui_widget_getstyle(rsge_ui_widget_t* widget,char* name,void** value)
 * \brief Gets a style value from the widget.
 * \param widget The pointer to the widget.
 * \param name The name of the style.
 * \param value The value of the style.
 * \return An error code.
 */
rsge_error_e rsge_ui_widget_getstyle(rsge_ui_widget_t* widget,char* name,void** value);

#endif