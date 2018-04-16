#include <rsge/assets.h>

rsge_error_e rsge_asset_find(rsge_assets_t* assets,rsge_asset_t** file,char* name) {
	for(size_t i = 0;i < assets->count;i++) {
		if(!strcmp(assets->files[i].name,name)) {
			*file = &assets->files[i];
			return RSGE_ERROR_NONE;
		}
	}
	return RSGE_ERROR_NOASSET;
}