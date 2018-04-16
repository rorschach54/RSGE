#ifndef __RSGE_GFX_SHAPE_H_
#define __RSGE_GFX_SHAPE_H_ 1

#include <rsge/gfx/gl.h>
#include <rsge/error.h>
#include <linmath.h>

#define RSGE_SHAPE_VERT_FLAG_COLOR (1 << 0)
#define RSGE_SHAPE_VERT_FLAG_FOG (1 << 1)

#define RSGE_SHAPE_FLAG_MULTIDISPLLST (1 << 0)

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
 * \struct rsge_shape_t rsge/gfx/shape.h
 * \brief Defines a shape.
 */
typedef struct {
	/**
	 * \brief Flags.
	 */
	uint8_t flags;

	/**
	 * \brief The display list used.
	 */
	GLuint list;

	/**
	 * \brief A 3-tuplet float that defines the position.
	 */
	vec3 pos;

	/**
	 * \brief The GL rendering mode to use.
	 */
	GLenum mode;

	/**
	 * \brief An array of vertices.
	 */
	rsge_shape_vert_t* vertices;

	/**
	 * \brief The number of vertices.
	 */
	int verticeCount;
} rsge_shape_t;

/**
 * \fn rsge_error_e rsge_shape_create(rsge_shape_t* shape,GLenum mode,rsge_shape_vert_t* vertices,int verticeCount,uint8_t flags)
 * \brief Creates a new shape.
 * \param shape The pointer to the shape.
 * \param mode The GL rendering mode to use.
 * \param vertices An array of vertices.
 * \param verticeCount The number of vertices.
 * \param flags Flags.
 * \return An error code.
 */
rsge_error_e rsge_shape_create(rsge_shape_t* shape,GLenum mode,rsge_shape_vert_t* vertices,int verticeCount,uint8_t flags);

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
