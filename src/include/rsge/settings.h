#ifndef __RSGE_SETTINGS_H_
#define __RSGE_SETTINGS_H_ 1

#include <rsge/platform/settings.h>
#include <rsge/error.h>
#include <libconfig.h>

/**
 * \fn rsge_error_e rsge_settings_getint(char* key,int* val)
 * \brief Gets a setting value.
 * \param key The setting key to use.
 * \param val The pointer to store the value.
 * \return An error code.
 */
rsge_error_e rsge_settings_getint(char* key,int* val);

/**
 * \fn rsge_error_e rsge_settings_setint(char* key,int val)
 * \brief Sets a setting value to a integer.
 * \param key The setting key to use.
 * \param val The value to write.
 * \return An error code.
 */
rsge_error_e rsge_settings_setint(char* key,int val);

/**
 * \fn rsge_error_e rsge_settings_getlibconfig(config_t* cfg)
 * \brief Gets the libconfig config_t varaible.
 * \param cfg The pointer to store the libconfig config_t variable.
 * \return An error code.
 */
rsge_error_e rsge_settings_getlibconfig(config_t* cfg);

/**
 * \fn rsge_error_e rsge_settings_loaddefault()
 * \brief Loads the default settings.
 * \return An error code.
 */
rsge_error_e rsge_settings_loaddefault();

#endif