#ifndef __RSGE_GFX_SHAPE_H_
#define __RSGE_GFX_SHAPE_H_ 1

#include <rsge/gfx/gl.h>
#include <rsge/gfx/shader.h>
#include <rsge/error.h>
#include <linmath.h>

/**
 * \struct rsge_shape_vert_t rsge/gfx/shape.h
 * \brief Defines a shape vertice.
 */
typedef struct {
	/**
	 * \brief A 3-tuplet float that defines the position.
	 */
	float x,y,z;

	/**
	 * \brief The color value of the vertice.
	 */
	float r,g,b,a;
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
	 * \brief Shader program.
	 */
	rsge_shaderprg_t shaderProg;
	
	GLint MVP;
	GLint vPos;
	GLint vCol;
	GLint position;
	
	GLuint vertexBuffer;
} rsge_shape_t;

/**
 * \fn rsge_error_e rsge_shape_create(rsge_shape_t* shape,rsge_shape_vert_t* vertices,int verticeCount)
 * \brief Creates a new shape.
 * \param shape The pointer to the shape.
 * \param vertices An array of vertices.
 * \param verticeCount The number of vertices.
 * \return An error code.
 */
rsge_error_e rsge_shape_create(rsge_shape_t* shape,rsge_shape_vert_t* vertices,int verticeCount);

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
