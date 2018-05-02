#ifndef __RSGE_UI_WIDGET_BUTTON_H_
#define __RSGE_UI_WIDGET_BUTTON_H_ 1

/** \file src/include/rsge/ui/widget/button.h
 * \brief UI button widget.
 * \author Spaceboy Ross
 */

#include <rsge/ui/widget.h>

/**
 * \fn rsge_error_e rsge_ui_widget_button_create(rsge_ui_widget_t* widget,rsge_ui_surface_t* ui,char* text)
 * \brief Creates a new UI Button widget.
 * \param[out] widget The pointer to the widget to use.
 * \param[in] ui The UI surface to use.
 * \param[in] text The text to render.
 * \return An error code.
 */
rsge_error_e rsge_ui_widget_button_create(rsge_ui_widget_t* widget,rsge_ui_surface_t* ui,char* text);

/**
 * \fn rrsge_error_e rsge_ui_widget_button_fromXMLNode(rsge_ui_widget_t* widget,rsge_ui_surface_t* ui,xmlDocPtr doc,xmlNodePtr node)
 * \brief Creates a new UI Button widget.
 * \param[out] widget The pointer to the widget to use.
 * \param[in] ui The UI surface to use.
 * \param[in] doc The XML document to use.
 * \param[in] node The root XML node to use.
 * \return An error code.
 */
rsge_error_e rsge_ui_widget_button_fromXMLNode(rsge_ui_widget_t* widget,rsge_ui_surface_t* ui,xmlDocPtr doc,xmlNodePtr node);

#endif
