#include <rsge/assets/rsge_assets.h>
#include <rsge/assets.h>
#include <list.h>
#include <log.h>

list_t* rsge_assets_locations;

rsge_error_e rsge_assets_init() {
	rsge_assets_locations = list_new();
	if(!rsge_assets_locations) {
		log_error("Failed to allocate memory");
		return RSGE_ERROR_MALLOC;
	}
	
	/* Register asset locations */
	rsge_asset_loc_t loc_rsge = {
		.assets = &rsge_assets,
		.name = "rsge"
	};
	rsge_error_e err = rsge_assets_registerLocation(&loc_rsge);
	if(err != RSGE_ERROR_NONE) {
		log_error("Failed to register location");
		return err;
	}
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_assets_uninit() {
	list_destroy(rsge_assets_locations);
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_assets_findloc(char* name,rsge_asset_loc_t** loc) {
    list_node_t* node;
    list_iterator_t* it = list_iterator_new(rsge_assets_locations,LIST_HEAD);
    if(it == NULL) return RSGE_ERROR_MALLOC;
    while((node = list_iterator_next(it))) {
        rsge_asset_loc_t* tmp_loc = (rsge_asset_loc_t*)node->val;
        if(!strcmp(tmp_loc->name,name)) {
            *loc = tmp_loc;
            list_iterator_destroy(it);
            return RSGE_ERROR_NONE;
        }
    }
    list_iterator_destroy(it);
	return RSGE_ERROR_INVALID_ASSET_LOC;
}

rsge_error_e rsge_assets_registerLocation(rsge_asset_loc_t* loc) {
	rsge_asset_loc_t* tmp_loc;
	rsge_error_e err = rsge_assets_findloc(loc->name,&tmp_loc);
	if(err == RSGE_ERROR_INVALID_ASSET_LOC) {
		list_node_t* node = list_node_new(loc);
    	if(!node) return RSGE_ERROR_MALLOC;
    	list_rpush(rsge_assets_locations,node);
		return RSGE_ERROR_NONE;
	} else if(err != RSGE_ERROR_NONE) return err;
	return RSGE_ERROR_INVALID_ASSET_LOC;
}

rsge_error_e rsge_asset_find(rsge_assets_t* assets,rsge_asset_t* file,char* name) {
	if(assets->find != NULL) return assets->find(assets,file,name);
	return RSGE_ERROR_NOASSET;
}

rsge_error_e rsge_asset_get(rsge_asset_t* file,char* name) {
	size_t splitter = 0;
	for(size_t i = 0;i < strlen(name);i++) {
		if(name[i] == '@') {
			splitter = i;
			break;
		}
	}
	char* locname = malloc(splitter-1);
	if(!locname) return RSGE_ERROR_MALLOC;
	memcpy(locname,name+splitter,splitter-1);
	
	rsge_asset_loc_t* loc;
	rsge_error_e err = rsge_assets_findloc(locname,&loc);
	if(err != RSGE_ERROR_NONE) {
		free(locname);
		return err;
	}
	
	err = rsge_asset_find(loc->assets,file,name);
	free(locname);
	return err;
}