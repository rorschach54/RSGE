#ifndef __RSGE_SETTINGS_H_
#define __RSGE_SETTINGS_H_ 1

#include <rsge/platform/settings.h>
#include <rsge/error.h>
#include <libconfig.h>

rsge_error_e rsge_settings_getlibconfig(config_t* cfg);
rsge_error_e rsge_settings_loaddefault();

#endif