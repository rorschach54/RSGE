#ifndef __RSGE_UI_WIDGET_LABEL_H_
#define __RSGE_UI_WIDGET_LABEL_H_ 1

#include <rsge/ui/widget.h>

/**
 * \fn rsge_error_e rsge_ui_widget_label_create(rsge_ui_widget_t* widget,rsge_ui_surface_t* ui,char* text)
 * \brief Creates a new UI Label widget.
 * \param widget The pointer to the widget to use.
 * \param ui The UI surface to use.
 * \param text The text to render.
 * \return An error code.
 */
rsge_error_e rsge_ui_widget_label_create(rsge_ui_widget_t* widget,rsge_ui_surface_t* ui,char* text);

rsge_error_e rsge_ui_widget_label_fromXMLNode(rsge_ui_widget_t* widget,rsge_ui_surface_t* ui,xmlDocPtr doc,xmlNodePtr node);

#endif