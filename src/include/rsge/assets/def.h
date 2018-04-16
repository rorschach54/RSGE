#ifndef __RSGE_ASSETS_DEF_H_
#define __RSGE_ASSETS_DEF_H_ 1

#include <rsge/assets/file.h>
#include <stdint.h>

typedef struct {
	rsge_asset_t* files;
	size_t count;
} rsge_assets_t;

#endif