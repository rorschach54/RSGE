#ifndef __RSGE_GFX_OBJECTS_VERT_H_
#define __RSGE_GFX_OBJECTS_VERT_H_ 1

/** \file src/include/rsge/gfx/objects/vert.h
 * \brief Vertex object
 * \author Spaceboy Ross
 */

#include <rsge/gfx/gl.h>
#include <rsge/error.h>
#include <linmath.h>
#include <string.h>

/** \struct rsge_vertex_t src/include/rsge/gfx/objects/vert.h rsge/gfx/objects/vert.h
 * \brief Vertex for the Vertex Object
 */
typedef struct {
    /**
     * \brief The position of the vertex.
     */
    vec3 pos;
    
    /**
     * \brief The normal value.
     */
    vec3 normal;
    
    /**
     * \brief The UV value.
     */
    vec2 uv;
    
    /**
     * \brief The tangent value.
     */
    vec3 tangent;
    
    /**
     * \brief The binormal value.
     */
    vec3 binormal;
} rsge_vertex_t;

/** \struct rsge_obj_vert_t src/include/rsge/gfx/objects/vert.h rsge/gfx/objects/vert.h
 * \brief Vertex Object
 */
typedef struct {
    /**
     * \brief The VAO id.
     */
    GLuint vao;
    
    /**
     * \brief The Buffer id.
     */
    GLuint buffer;
} rsge_obj_vert_t;

/**
 * \fn rsge_error_e rsge_obj_vert_create(rsge_obj_vert_t* vert,rsge_vertex_t* vertices,size_t vertexCount)
 * \brief Creates a new vertex object.
 * \param[out] vert The pointer to store the vertex object.
 * \param[in] vertices The vertices to use.
 * \param[in] vertexCount The number of vertices.
 * \return An error code.
 */
rsge_error_e rsge_obj_vert_create(rsge_obj_vert_t* vert,rsge_vertex_t* vertices,size_t vertexCount);

/**
 * \fn rsge_error_e rsge_obj_vert_destroy(rsge_obj_vert_t* vert)
 * \brief Destroyes vertex object.
 * \param[out] vert The pointer to store the vertex object.
 * \return An error code.
 */
rsge_error_e rsge_obj_vert_destroy(rsge_obj_vert_t* vert);

#endif