#ifndef __RSGE_GFX_TILING_ENGINE_H_
#define __RSGE_GFX_TILING_ENGINE_H_ 1

/** \file src/include/rsge/gfx/tiling-engine.h
 * \brief Tiling engine stuff
 * \author Spaceboy Ross
 */

#include <rsge/gfx/gl.h>
#include <rsge/gfx/surface.h>
#include <rsge/error.h>
#include <linmath.h>

/**
 * Enable a top-down rendering.
 */
#define RSGE_TILING_FLAG_TOPDOWN (1 << 0)

/** \struct rsge_tiling_t src/include/rsge/gfx/tiling-engine.h rsge/gfx/tiling-engine.h
 * \brief A tiling map.
 */
typedef struct {
	/**
	 * \brief Flags enables features.
	 */
	int flags;

    /**
     * \brief The map of the tiling.
     */
    int** map;
    
    /**
     * \brief The width of the map.
     */
    int width;
    
    /**
     * \brief The height of the map.
     */
    int height;
    
    /**
     * \brief The textures used.
     */
    GLuint* textures;
    
    /**
     * \brief The count texture.
     */
    int textureCount;
    
    /**
     * \brief The base position.
     */
    vec3 pos;
    
    /**
     * \brief The display list used.
     */
    GLuint list;
} rsge_tiling_t;

/**
 * \fn rsge_error_e rsge_tiling_create(rsge_tiling_t* tiling,int** map,int width,int height,rsge_surface_t** surfaces,int surfaceCount,int flags)
 * \brief Creates a new tiling object.
 * \param[out] tiling The pointer to store the tiling object.
 * \param[in] map The map to use.
 * \param[in] width The width of the map.
 * \param[in] height The height of the map.
 * \param[in] surfaces The surfaces to use.
 * \param[in] surfaceCount The number of surfaces.
 * \param[in] flags The flags to use.
 * \return An error code.
 */
rsge_error_e rsge_tiling_create(rsge_tiling_t* tiling,int** map,int width,int height,rsge_surface_t** surfaces,int surfaceCount,int flags);

/**
 * \fn rsge_error_e rsge_tiling_destroy(rsge_tiling_t* tiling)
 * \brief Destroyes a tiling object.
 * \param[out] tiling The pointer to store the tiling object.
 * \return An error code.
 */
rsge_error_e rsge_tiling_destroy(rsge_tiling_t* tiling);

/**
 * \fn rsge_error_e rsge_tiling_render(rsge_tiling_t* tiling)
 * \brief Renders a tiling object.
 * \param[out] tiling The pointer to store the tiling object.
 * \return An error code.
 */
rsge_error_e rsge_tiling_render(rsge_tiling_t* tiling);

#endif
