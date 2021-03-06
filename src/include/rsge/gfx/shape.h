#ifndef __RSGE_GFX_SHAPE_H_
#define __RSGE_GFX_SHAPE_H_ 1

/** \file src/include/rsge/gfx/shape.h
 * \brief Shape stuff
 * \author Spaceboy Ross
 */

#include <rsge/gfx/gl.h>
#include <rsge/gfx/shader.h>
#include <rsge/error.h>
#include <linmath.h>
#include <stdint.h>

/**
 * \brief Enable color.
 */
#define RSGE_SHAPE_VERT_FLAG_COLOR (1 << 1)
/**
 * \brief Enable secondary color.
 */
#define RSGE_SHAPE_VERT_FLAG_2NDCOLOR (2 << 1)
/**
 * \brief Enable fog.
 */
#define RSGE_SHAPE_VERT_FLAG_FOG (3 << 1)
/**
 * \brief Enable texturing.
 */
#define RSGE_SHAPE_VERT_FLAG_TEXTURE (4 << 1)

/**
 * \brief Enable shadows.
 */
#define RSGE_SHAPE_FLAG_SHADOWS (1 << 1)

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

	/**
	 * \brief The texture coordinate value. Use when RSGE_SHAPE_VERT_FLAG_TEXTURE flag is set.
	 */
	vec3 textCoord;
} rsge_shape_vert_t;

/**
 * \struct rsge_shape_t rsge/gfx/shape.h
 * \brief Defines a shape.
 */
typedef struct {
	/**
	 * \brief flags.
	 */
	uint8_t flags;

	/**
	 * \brief The rotation as a quaternion.
	 */
    quat rotation;

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

	/**
	 * \brief The OpenGL display list used.
	 */
	GLuint list;

	/**
	 * \brief The glBegin mode to use.
	 */
	GLenum mode;

	/**
	 * \brief The texture ID, used when it's value is not zero.
	 */
	GLuint texture;

	/**
	 * \brief The texture mode to use.
	 */
	GLenum textureMode;
} rsge_shape_t;

/**
 * \fn rsge_error_e rsge_shape_create(rsge_shape_t* shape,GLenum mode,rsge_shape_vert_t* vertices,int verticeCount,rsge_shape_mat_t* materials,int materialCount,uint8_t flags)
 * \brief Creates a new shape.
 * \deprecated Use the vertex object stuff that has been implemented with ELGLR.
 * \param[out] shape The pointer to the shape.
 * \param[in] mode The GL rendering mode to use.
 * \param[in] vertices An array of vertices.
 * \param[in] verticeCount The number of vertices.
 * \param[in] materials The materials to use.
 * \param[in] materialCount The number of materials.
 * \param[in] flags The flags to use.
 * \return An error code.
 */
rsge_error_e rsge_shape_create(rsge_shape_t* shape,GLenum mode,rsge_shape_vert_t* vertices,int verticeCount,rsge_shape_mat_t* materials,int materialCount,uint8_t flags) __attribute__ ((deprecated));

/**
 * \fn rsge_error_e rsge_shape_destroy(rsge_shape_t* shape)
 * \brief Destroyes the shape.
 * \deprecated Use the vertex object stuff that has been implemented with ELGLR.
 * \param[out] shape The pointer to the shape.
 * \return An error code.
 */
rsge_error_e rsge_shape_destroy(rsge_shape_t* shape) __attribute__ ((deprecated));

/**
 * \fn rsge_error_e rsge_shape_render(rsge_shape_t* shape)
 * \brief Renders the shape.
 * \deprecated Use the vertex object stuff that has been implemented with ELGLR.
 * \param[out] shape The pointer to the shape.
 * \return An error code.
 */
rsge_error_e rsge_shape_render(rsge_shape_t* shape) __attribute__ ((deprecated));

#endif