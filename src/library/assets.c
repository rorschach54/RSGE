#include <rsge/assets.h>

rsge_error_e rsge_asset_find(rsge_assets_t* assets,rsge_asset_file_t** file,char* name) {
	for(size_t i = 0;i < assets->count;i++) {
		rsge_asset_file_t* asset = assets->files[i];
		if(asset == NULL) continue;
		if(!strcmp(asset->name,name)) {
			*file = asset;
			return RSGE_ERROR_NONE;
		}
	}
	return RSGE_ERROR_NOASSET;
}