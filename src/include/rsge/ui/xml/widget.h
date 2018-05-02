#ifndef __RSGE_UI_XML_WIDGET_H_
#define __RSGE_UI_XML_WIDGET_H_ 1

/** \file src/include/rsge/ui/xml/widget.h
 * \brief UI stuff through XML.
 * \author Spaceboy Ross
 */

#include <libxml/parser.h>
#include <rsge/ui/widget.h>
#include <rsge/assets.h>
#include <rsge/error.h>

/**
 * \fn rsge_error_e rsge_ui_widget_fromAssets(rsge_ui_widget_t* widget,rsge_ui_surface_t* ui,xmlDocPtr doc,xmlNodePtr node)
 * \brief Loads a widget from a XML node.
 * \param[out] widget The pointer to the widget.
 * \param[in] ui The UI surface to use.
 * \param[in] doc The XML document.
 * \param[in] node The XML node.
 * \return An error.
 */
rsge_error_e rsge_ui_widget_fromXMLNode(rsge_ui_widget_t* widget,rsge_ui_surface_t* ui,xmlDocPtr doc,xmlNodePtr node);

/**
 * \fn rsge_error_e rsge_ui_widget_fromAssets(rsge_ui_widget_t* widget,rsge_ui_surface_t* ui,rsge_asset_t* asset)
 * \brief Loads a widget from a XML asset file.
 * \param[out] widget The pointer to the widget.
 * \param[in] ui The UI surface to use.
 * \param[in] asset The asset to load from.
 * \return An error.
 */
rsge_error_e rsge_ui_widget_fromAssets(rsge_ui_widget_t* widget,rsge_ui_surface_t* ui,rsge_asset_t* asset);

#endif
