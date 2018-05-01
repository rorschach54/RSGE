#ifndef __RSGE_GFX_TILING_ENGINE_H_
#define __RSGE_GFX_TILING_ENGINE_H_ 1

#include <rsge/gfx/gl.h>
#include <rsge/gfx/surface.h>
#include <rsge/error.h>
#include <linmath.h>

#define RSGE_TILING_FLAG_TOPDOWN (1 << 0)

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
} rsge_tiling_t;

/**
 * \fn rsge_error_e rsge_tiling_create(rsge_tiling_t* tiling,int** map,int width,int height,rsge_surface_t** surfaces,int surfaceCount,int flags)
 * \brief Creates a new tiling object.
 * \param tiling The pointer to store the tiling object.
 * \param map The map to use.
 * \param width The width of the map.
 * \param height The height of the map.
 * \param surfaces The surfaces to use.
 * \param surfaceCount The number of surfaces.
 * \param flags The flags to use.
 * \return An error code.
 */
rsge_error_e rsge_tiling_create(rsge_tiling_t* tiling,int** map,int width,int height,rsge_surface_t** surfaces,int surfaceCount,int flags);

/**
 * \fn rsge_error_e rsge_tiling_destroy(rsge_tiling_t* tiling)
 * \brief Destroyes a tiling object.
 * \param tiling The pointer to store the tiling object.
 * \return An error code.
 */
rsge_error_e rsge_tiling_destroy(rsge_tiling_t* tiling);

/**
 * \fn rsge_error_e rsge_tiling_render(rsge_tiling_t* tiling)
 * \brief Renders a tiling object.
 * \param tiling The pointer to store the tiling object.
 * \return An error code.
 */
rsge_error_e rsge_tiling_render(rsge_tiling_t* tiling);

#endif
