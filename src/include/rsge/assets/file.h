#ifndef __RSGE_ASSETS_FILE_H_
#define __RSGE_ASSETS_FILE_H_ 1

/** \file src/include/rsge/assets/file.h
 * \brief Asset stuff.
 * \author Spaceboy Ross
 */

#include <rsge/error.h>
#include <stdint.h>
#include <string.h>

/** \struct rsge_asset_t src/include/rsge/assets/file.h rsge/assets/file.h
 * \brief An asset.
 */
typedef struct {
	/**
	 * \brief The name of the asset.
	 */
	char* name;
	/**
	 * \brief The asset's contents.
	 */
	char* data;
	/**
	 * \brief The asset's size in bytes.
	 */
	uint32_t size;
} rsge_asset_t;

#endif
