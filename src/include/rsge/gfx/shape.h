#ifndef __RSGE_GFX_SHAPE_H_
#define __RSGE_GFX_SHAPE_H_ 1

#include <rsge/gfx/gl.h>
#include <rsge/gfx/shader.h>
#include <rsge/error.h>
#include <linmath.h>
#include <stdint.h>

#define RSGE_SHAPE_VERT_FLAG_COLOR (1 << 1)
#define RSGE_SHAPE_VERT_FLAG_2NDCOLOR (2 << 1)
#define RSGE_SHAPE_VERT_FLAG_FOG (3 << 1)

/**
 * \struct rsge_shape_mat_t rsge/gfx/shape.h
 * \brief Defines a shape material.
 */
typedef struct {
	/**
	 * \brief The face of the shape to use.
	 */
	GLenum face;
	
	/**
	 * \brief Sets the type of material to use. Must be one of GL_AMBIENT, GL_DIFFUSE, GL_SPECULAR, GL_EMISSION, GL_SHININESS, GL_AMBIENT_AND_DIFFUSE, or GL_COLOR_INDEXES.
	 */
	GLenum type;
	
	/**
	 * \brief The values to use.
	 */
	GLint* values;
} rsge_shape_mat_t;

/**
 * \struct rsge_shape_vert_t rsge/gfx/shape.h
 * \brief Defines a shape vertice.
 */
typedef struct {
	/**
	 * \brief flags.
	 */
	uint8_t flags;
	
	/**
	 * \brief A 3-tuplet float that defines the position.
	 */
	vec3 pos;

	/**
	 * \brief The color value of the vertice. Use when RSGE_SHAPE_VERT_FLAG_COLOR flag is set.
	 */
	float color[4];
	
	/**
	 * \brief The secondary color value of the vertice. Use when RSGE_SHAPE_VERT_FLAG_2NDCOLOR flag is set.
	 */
	float color2nd[4];
	
	/**
	 * \brief The fog coord value. Use when RSGE_SHAPE_VERT_FLAG_FOG flag is set.
	 */
	float fogCoord;
} rsge_shape_vert_t;

/**
 * \struct rsge_shape_t rsge/gfx/shape.h
 * \brief Defines a shape.
 */
typedef struct {

	/**
	 * \brief A 3-tuplet float that defines the position.
	 */
	vec3 pos;

	/**
	 * \brief An array of vertices.
	 */
	rsge_shape_vert_t* vertices;

	/**
	 * \brief The number of vertices.
	 */
	int verticeCount;
	
	/**
	 * \brief An array of materials.
	 */
	rsge_shape_mat_t* materials;

	/**
	 * \brief The number of materials.
	 */
	int materialCount;
	
	/**
	 * \brief Shader program.
	 */
	rsge_shaderprg_t shaderProg;
	
	GLuint list;
	GLenum mode;
} rsge_shape_t;

/**
 * \fn rsge_error_e rsge_shape_create(rsge_shape_t* shape,GLenum mode,rsge_shape_vert_t* vertices,int verticeCount,rsge_shape_mat_t* materials,int materialCount)
 * \brief Creates a new shape.
 * \param shape The pointer to the shape.
 * \param mode The GL rendering mode to use.
 * \param vertices An array of vertices.
 * \param verticeCount The number of vertices.
 * \return An error code.
 */
rsge_error_e rsge_shape_create(rsge_shape_t* shape,GLenum mode,rsge_shape_vert_t* vertices,int verticeCount,rsge_shape_mat_t* materials,int materialCount);

/**
 * \fn rsge_error_e rsge_shape_destroy(rsge_shape_t* shape)
 * \brief Destroyes the shape.
 * \param shape The pointer to the shape.
 * \return An error code.
 */
rsge_error_e rsge_shape_destroy(rsge_shape_t* shape);

/**
 * \fn rsge_error_e rsge_shape_render(rsge_shape_t* shape)
 * \brief Renders the shape.
 * \param shape The pointer to the shape.
 * \return An error code.
 */
rsge_error_e rsge_shape_render(rsge_shape_t* shape);

#endif
