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
	rsge_asset_loc_t* loc_rsge = malloc(sizeof(rsge_asset_loc_t));
	if(!loc_rsge) return RSGE_ERROR_NONE;
	loc_rsge->assets = &rsge_assets;
	loc_rsge->name = "rsge";
	rsge_error_e err = rsge_assets_registerLocation(loc_rsge);
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
		log_debug("Found asset location %s",((rsge_asset_loc_t*)node->val)->name);
		if(!strcmp(((rsge_asset_loc_t*)node->val)->name,name)) {
			*loc = ((rsge_asset_loc_t*)node->val);
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
	if(splitter == 0) return RSGE_ERROR_INVALID_ASSET_LOC;
	char* locname = malloc(splitter);
	if(!locname) return RSGE_ERROR_MALLOC;
	strncpy(locname,name,splitter);
	locname[splitter] = 0;

	char* path = malloc(strlen(name)-splitter);
	if(!path) {
		free(locname);
		return RSGE_ERROR_MALLOC;
	}
	memset(path,0,(strlen(name)-splitter));
	strncpy(path,name+(splitter+1),strlen(name)-splitter);

	log_debug("Looking for asset %s in %s",path,locname);
	
	rsge_asset_loc_t* loc;
	rsge_error_e err = rsge_assets_findloc(locname,&loc);
	if(err != RSGE_ERROR_NONE) {
		log_error("%s is not a valid asset location",locname);
		free(locname);
		free(path);
		return err;
	}
	
	err = rsge_asset_find(loc->assets,file,path);
	free(locname);
	free(path);
	return err;
}

rsge_error_e rsge_asset_read(char* name,char** buffer,size_t* count) {
	rsge_asset_t file;
	rsge_error_e err = rsge_asset_get(&file,name);
	if(err != RSGE_ERROR_NONE) return err;
	*count = file.size;
	*buffer = calloc(1,file.size);
	if(buffer == NULL) return RSGE_ERROR_MALLOC;
	memset(*buffer,0,file.size);
	strncpy(*buffer,file.data,file.size);
	return RSGE_ERROR_NONE;
}
