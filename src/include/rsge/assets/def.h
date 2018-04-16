#ifndef __RSGE_ASSETS_DEF_H_
#define __RSGE_ASSETS_DEF_H_ 1

#include <rsge/assets/file.h>
#include <rsge/error.h>
#include <stdint.h>

typedef struct rsge_asset {
	rsge_error_e (*find)(struct rsge_asset* assets,rsge_asset_t* file,char* key);
} rsge_assets_t;

#endif