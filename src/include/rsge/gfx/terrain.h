#ifndef __RSGE_GFX_TERRAIN_H_
#define __RSGE_GFX_TERRAIN_H_ 1

/** \file src/include/rsge/gfx/terrain.h
 * \brief Terrain stuff
 * \author Spaceboy Ross
 */

#include <libxml/parser.h>
#include <rsge/gfx/gl.h>
#include <rsge/gfx/surface.h>
#include <rsge/error.h>
#include <linmath.h>
#include <string.h>

/** \struct rsge_terrain_point_t src/include/rsge/gfx/terrain.h rsge/gfx/terrain.h
 * \brief A point in the terrain.
 */
typedef struct {
    /**
     * \brief The data of the terrain point
     */
    float data[3];
    
    /**
     * \brief The texture index.
     */
    int texture;
} rsge_terrain_point_t;

/** \struct rsge_terrain_t src/include/rsge/gfx/terrain.h rsge/gfx/terrain.h
 * \brief Terrain.
 */
typedef struct {
    /**
     * \brief The terrain points.
     */
    rsge_terrain_point_t** points;
    
    /**
     * \brief The terrain width.
     */
    int width;
    
    /**
     * \brief The terrain length.
     */
    int length;
    
    /**
     * \brief The textures.
     */
    GLuint* textures;
    
    /**
     * \brief The texture count.
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
} rsge_terrain_t;

/**
 * \fn rsge_error_e rsge_terrain_create(rsge_terrain_t* terrain,rsge_terrain_point_t** points,int length,int width,rsge_surface_t** surfaces,int surfaceCount)
 * \brief Creates a new terrain.
 * \param[out] terrain The pointer to store the terrain.
 * \param[in] points The terrain points.
 * \param[in] length The length of the terrain.
 * \param[in] width The width of the terrain.
 * \param[in] surfaces The surfaces to use.
 * \param[in] surfaceCount The number of surfaces.
 * \return An error code.
 */
rsge_error_e rsge_terrain_create(rsge_terrain_t* terrain,rsge_terrain_point_t** points,int length,int width,rsge_surface_t** surfaces,int surfaceCount);

/**
 * \fn rsge_error_e rsge_terrain_destroy(rsge_terrain_t* terrain)
 * \brief Destroyes the terrain.
 * \param[out] terrain The pointer to store the terrain.
 * \return An error code.
 */
rsge_error_e rsge_terrain_destroy(rsge_terrain_t* terrain);

/**
 * \fn rsge_error_e rsge_terrain_render(rsge_terrain_t* terrain)
 * \brief Renders the terrain.
 * \param[out] terrain The pointer to store the terrain.
 * \return An error code.
 */
rsge_error_e rsge_terrain_render(rsge_terrain_t* terrain);

#endif
