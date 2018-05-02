#ifndef __RSGE_UI_H_
#define __RSGE_UI_H_ 1

/** \file src/include/rsge/ui.h
 * \brief UI component.
 * \author Spaceboy Ross
 */

#include <rsge/ui/widget.h>
#include <rsge/error.h>

/**
 * \fn rsge_error_e rsge_ui_init()
 * \brief Initializes the UI engine. Do not run this in your code, this is automatically called.
 * \return An error code.
 */
rsge_error_e rsge_ui_init();

/**
 * \fn rsge_error_e rsge_ui_deinit()
 * \brief Deinitializes the UI engine. Do not run this in your code, this is automatically called.
 * \return An error code.
 */
rsge_error_e rsge_ui_deinit();

/**
 * \fn rsge_error_e rsge_ui_registerWidget(rsge_ui_widget_type_t* type)
 * \brief Registers a new widget.
 * \param[in] type The widget type to register.
 * \return An error code.
 */
rsge_error_e rsge_ui_registerWidget(rsge_ui_widget_type_t* type);

/**
 * \fn rsge_error_e rsge_ui_getregisteredwidget(char* tag,rsge_ui_widget_type_t** type)
 * \brief Gets a registered widget by tag.
 * \param[in] tag The tag to search for.
 * \param[out] type The pointer to the type.
 * \return An error code.
 */
rsge_error_e rsge_ui_getregisteredwidget(char* tag,rsge_ui_widget_type_t** type);

#endif
