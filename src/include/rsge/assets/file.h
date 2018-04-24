#ifndef __RSGE_ASSETS_FILE_H_
#define __RSGE_ASSETS_FILE_H_ 1

#include <rsge/error.h>
#include <stdint.h>
#include <string.h>

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
