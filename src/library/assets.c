#include <rsge/assets.h>

rsge_error_e rsge_asset_find(rsge_assets_t* assets,rsge_asset_t* file,char* name) {
	if(assets->find != NULL) return assets->find(assets,file,name);
	return RSGE_ERROR_NOASSET;
}