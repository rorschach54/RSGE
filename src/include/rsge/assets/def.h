#ifndef __RSGE_ASSETS_DEF_H_
#define __RSGE_ASSETS_DEF_H_ 1

/** \file src/include/rsge/assets/def.h
 * \brief Assets stuff.
 * \author Spaceboy Ross
 */

#include <rsge/assets/file.h>
#include <rsge/error.h>
#include <stdint.h>

/** \struct rsge_assets_t src/include/rsge/assets/def.h rsge/assets/def.h
 * \brief Assets structure.
 */
typedef struct rsge_assets {
	/**
	 * \brief Function to find an asset.
	 */
	rsge_error_e (*find)(struct rsge_assets* assets,rsge_asset_t* file,char* key);

	/**
	 * \brief Implementation data.
	 */
	void* impl;
} rsge_assets_t;

#endif
