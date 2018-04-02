#ifndef __RSGE_PLATFORM_SETTINGS_H_
#define __RSGE_PLATFORM_SETTINGS_H_ 1

#include <rsge/error.h>
#include <libconfig.h>

rsge_error_e rsge_settings_load();
rsge_error_e rsge_settings_save();

#endif