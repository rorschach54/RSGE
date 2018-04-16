#ifndef __RSGE_ASSETS_H_
#define __RSGE_ASSETS_H_ 1

#include <rsge/assets/def.h>
#include <rsge/assets/file.h>
#include <rsge/error.h>

rsge_error_e rsge_asset_find(rsge_assets_t* assets,rsge_asset_t** file,char* name);

#endif