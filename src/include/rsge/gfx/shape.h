#ifndef __RSGE_GFX_SHAPE_H_
#define __RSGE_GFX_SHAPE_H_ 1

#include <rsge/gfx/gl.h>
#include <rsge/error.h>
#include <linmath.h>

#define RSGE_SHAPE_VERT_FLAG_COLOR (1 << 0)
#define RSGE_SHAPE_VERT_FLAG_FOG (1 << 1)

/**
 * \struct rsge_shape_vert_t rsge/gfx/shape.h
 * \brief Defines a shape vertice.
 */
typedef struct {
	/**
	 * \brief Flags that enable specific parts of the vertice.
	 */
	uint8_t flags;

	/**
	 * \brief A 3-tuplet float that defines the position.
	 */
	vec3 pos;

	/**
	 * \brief The color value of the vertice, available when RSGE_SHAPE_VERT_FLAG_COLOR flag is set.
	 */
	float color[4];

	/**
	 * \brief The fog coord value of the vertice, available when RSGE_SHAPE_VERT_FLAG_FOG flag is set.
	 */
	float fogCoord;
} rsge_shape_vert_t;

/**
 * \fn rsge_error_e rsge_shape_generate(GLuint* list,GLenum mode,rsge_shape_vert_t* vertices,int verticeCount)
 * \brief Generates a shape.
 * \param list A pointer to the variable to use for a display list
 * \param mode The GL rendering mode to use.
 * \param vertices An array of vertices.
 * \param verticeCount The number of vertices.
 * \return An error code.
 */
rsge_error_e rsge_shape_generate(GLuint* list,GLenum mode,rsge_shape_vert_t* vertices,int verticeCount);

#endif
