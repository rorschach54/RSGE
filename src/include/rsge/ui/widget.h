#ifndef __RSGE_UI_WIDGET_H_
#define __RSGE_UI_WIDGET_H_ 1

/** \file src/include/rsge/ui/widget.h
 * \brief UI widget stuff.
 * \author Spaceboy Ross
 */

#include <rsge/gfx/colors.h>
#include <rsge/gfx/font.h>
#include <rsge/ui/surface.h>
#include <libxml/parser.h>
#include <list.h>
#include <string.h>

/** \struct rsge_ui_widget_style_t src/include/rsge/ui/widget.h rsge/ui/widget.h
 * \brief UI widget style.
 */
typedef struct {
	/**
	 * \brief Style name.
	 */
    char* name;
	/**
	 * \brief Style value.
	 */
    void* value;
} rsge_ui_widget_style_t;

/** \struct rsge_ui_widget_event_t src/include/rsge/ui/widget.h rsge/ui/widget.h
 * \brief UI widget event.
 */
typedef struct {
    /**
     * \brief Widget styling.
     */
    xmlNodePtr stylesNode;
    
    /**
     * \brief The type of the event.
     */
    char* type;
} rsge_ui_widget_event_t;

/** \struct rsge_ui_widget_t src/include/rsge/ui/widget.h rsge/ui/widget.h
 * \brief UI widget
 */
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

/** \struct rsge_ui_widget_type_t src/include/rsge/ui/widget.h rsge/ui/widget.h
 * \brief UI widget type.
 */
typedef struct {
    /**
     * \brief XML tag name.
     */
    char* tag;
    
	/**
	 * \brief The callback that is used to create the widget from an XML node.
	 */
    rsge_error_e (*fromXMLNode)(rsge_ui_widget_t* widget,rsge_ui_surface_t* ui,xmlDocPtr doc,xmlNodePtr node);
} rsge_ui_widget_type_t;

/**
 * \fn rsge_error_e rsge_ui_widget_loadStyles(rsge_ui_widget_t* widget,rsge_ui_surface_t* ui,xmlDocPtr doc,xmlNodePtr node)
 * \brief Load the styles from an XML node.
 * \param[out] widget The pointer to the widget.
 * \param[in] ui The UI surface to use.
 * \param[in] doc The XML document to use for base widget.
 * \param[in] node The XML node to use.
 * \return An error code.
 */
rsge_error_e rsge_ui_widget_loadStyles(rsge_ui_widget_t* widget,rsge_ui_surface_t* ui,xmlDocPtr doc,xmlNodePtr node);

/**
 * \fn rsge_error_e rsge_ui_widget_create(rsge_ui_widget_t* widget,rsge_ui_surface_t* ui,xmlDocPtr doc,xmlNodePtr node)
 * \brief Creates a generic widget.
 * \param[out] widget The pointer to the widget.
 * \param[in] ui The UI surface to use.
 * \param[in] doc The XML document to use for base widget.
 * \param[in] node The XML node to use.
 * \return An error code.
 */
rsge_error_e rsge_ui_widget_create(rsge_ui_widget_t* widget,rsge_ui_surface_t* ui,xmlDocPtr doc,xmlNodePtr node);

/**
 * \fn rsge_error_e rsge_ui_widget_getstyle(rsge_ui_widget_t* widget,char* name,void** value)
 * \brief Gets a style value from the widget.
 * \param[out] widget The pointer to the widget.
 * \param[in] name The name of the style.
 * \param[out] value The value of the style.
 * \return An error code.
 */
rsge_error_e rsge_ui_widget_getstyle(rsge_ui_widget_t* widget,char* name,void** value);

/**
 * \fn rsge_error_e rsge_ui_widget_getevent(rsge_ui_widget_t* widget,char* name,rsge_ui_widget_event_t*** events,int* count)
 * \brief Gets all of the events that matched name.
 * \param[out] widget The pointer to the widget.
 * \param[in] name The name of the event.
 * \param[out] events The pointer to store the events.
 * \param[out] count The pointer to store the number of events.
 */
rsge_error_e rsge_ui_widget_getevent(rsge_ui_widget_t* widget,char* name,rsge_ui_widget_event_t*** events,int* count);

#endif
