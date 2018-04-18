#ifndef __RSGE_ASSETS_H_
#define __RSGE_ASSETS_H_ 1

#include <rsge/assets/def.h>
#include <rsge/assets/file.h>
#include <rsge/error.h>

/**
 * \struct rsge_asset_loc_t rsge/assets.h
 * \brief An asset location.
 */
typedef struct {
    /**
     * \brief The assets.
     */
    rsge_assets_t* assets;
    
    /**
     * \brief The name of the location.
     */
    char* name;
} rsge_asset_loc_t;

/**
 * \fn rsge_error_e rsge_assets_init()
 * \brief Initializes the assets. This automatically runs.
 * \return An error code.
 */
rsge_error_e rsge_assets_init();

/**
 * \fn rsge_error_e rsge_assets_uninit()
 * \brief Uninitializes the assets. This automatically runs.
 * \return An error code.
 */
rsge_error_e rsge_assets_uninit();

/**
 * \fn rsge_error_e rsge_assets_findloc(char* name,rsge_asset_loc_t** loc)
 * \brief Finds an asset location.
 * \param name The location name.
 * \param loc The asset location entry.
 * \return An error code.
 */
rsge_error_e rsge_assets_findloc(char* name,rsge_asset_loc_t** loc);

/**
 * \fn rsge_error_e rsge_assets_registerLocation(rsge_asset_loc_t* loc)
 * \brief Registers a new asset location.
 * \param loc The asset location entry.
 * \return An error code.
 */
rsge_error_e rsge_assets_registerLocation(rsge_asset_loc_t* loc);

/**
 * \fn rsge_error_e rsge_asset_find(rsge_assets_t* assets,rsge_asset_t* file,char* name)
 * \brief Finds an asset inside of the asset location.
 * \param assets The location to use.
 * \param file The pointer to the asset.
 * \param name The path of the asset.
 * \return An error code.
 */
rsge_error_e rsge_asset_find(rsge_assets_t* assets,rsge_asset_t* file,char* name);

/**
 * \fn rsge_error_e rsge_asset_get(rsge_asset_t* file,char* name)
 * \brief Gets an asset from a location. Name looks like "location@path"
 * \param file The pointer to the asset.
 * \param name The location of the asset.
 * \return An error code.
 */
rsge_error_e rsge_asset_get(rsge_asset_t* file,char* name);

#endif