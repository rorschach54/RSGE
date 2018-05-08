#ifndef __RSGE_UI_H_
#define __RSGE_UI_H_ 1

/** \file src/include/rsge/ui.h
 * \brief UI component.
 * \author Spaceboy Ross
 */

#include <rsge/error.h>
#include <AntTweakBar.h>

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

#endif