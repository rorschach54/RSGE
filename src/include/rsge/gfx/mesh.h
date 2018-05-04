#ifndef __RSGE_GFX_MESH_H_
#define __RSGE_GFX_MESH_H_ 1

/** \file src/include/rsge/gfx/mesh.h
 * \brief Mesh stuff
 * \author Spaceboy Ross
 */

#include <rsge/gfx/objects/vert.h>
#include <rsge/gfx/colors.h>
#include <rsge/error.h>
#include <linmath.h>
#include <list.h>
#include <stdbool.h>

/** \struct rsge_material_t src/include/rsge/gfx/mesh.h rsge/gfx/mesh.h
 * \brief Material
 */
typedef struct {
    /**
     * \brief The name of the material.
     */
    char* name;
    
    /**
     * \brief The diffuse value.
     */
    rsge_color_rgba_t diffuse;
    
    /**
     * \brief The specular value.
     */
    rsge_color_rgba_t specular;
    
    /**
     * \brief The specular exponent value.
     */
    float specularExponent;
    
    /**
     * \brief The emissivity value.
     */
    rsge_color_rgba_t emissivity;
	
	/**
	 * \brief The diffuse map ID.
	 */
    int diffuseMapID;
	
	/**
	 * \brief The specual map ID.
	 */
	int specularMapID;
	
	/**
	 * \brief The emissivity map ID.
	 */
	int emissivityMapID;
	
	/**
	 * \brief The bump map ID.
	 */
	int bumpMapID;
} rsge_material_t;

/** \struct rsge_mesh_t src/include/rsge/gfx/mesh.h rsge/gfx/mesh.h
 * \brief Mesh
 */
typedef struct {
    /**
     * \brief The material of the mesh.
     */
    rsge_material_t material;
    
    /**
     * \brief The vertices of the mesh.
     */
    list_t* vertices;
    
    /**
     * \brief The triangles of the mesh.
     */
    list_t* triangles;
    
    /**
     * \brief The normals of the mesh.
     */
    list_t* normals;
} rsge_mesh_t;

/** \struct rsge_triangle_t src/include/rsge/gfx/mesh.h rsge/gfx/mesh.h
 * \brief Triangle
 */
typedef struct {
    /**
     * \brief The parent material.
     */
    rsge_material_t parent;
    
    /**
     * \brief Is the triangle textured.
     */
    bool isTextured;
    
    /**
     * \brief Index of this vertex in the parent
     */
    int vertIndex1;
    /**
     * \brief Index of this vertex in the parent
     */
	int vertIndex2;
    /**
     * \brief Index of this vertex in the parent
     */
	int vertIndex3;
	
	/**
	 * \brief The texture coordinates
	 */
	vec2 uv1;
	/**
	 * \brief The texture coordinates
	 */
	vec2 uv2;
	/**
	 * \brief The texture coordinates
	 */
	vec2 uv3;
	
	/**
	 * \brief Tangent (U expressed in object coordinates)
	 */
	vec3 tangent;
	/**
	 * \brief Binormal (V expressed in object coordinates)
	 */
	vec3 binormal;
} rsge_triangle_t;

#endif