#ifndef __RSGE_GFX_OBJECTS_UNIFBUFF_H_
#define __RSGE_GFX_OBJECTS_UNIFBUFF_H_ 1

/** \file src/include/rsge/gfx/objects/unifbuff.h
 * \brief Uniform Buffer Object.
 * \author Spaceboy Ross
 */

#include <rsge/gfx/gl.h>
#include <rsge/gfx/shader.h>
#include <rsge/error.h>

/** \struct rsge_obj_unifbuff_t src/include/rsge/gfx/objects/unifbuff.h rsge/gfx/objects/unifbuff.h
 * \brief Uniform Buffer Object
 */
typedef struct {
    /**
     * \brief The OpenGL Uniform Buffer Object id.
     */
    GLuint ubo;
	/**
	 * \brief Block index count.
	 */
	int blockIndexCount;
	/**
	 * \brief The block indexes.
	 */
	GLuint* blockIndexes;
} rsge_obj_unifbuff_t;

/**
 * \fn rsge_error_e rsge_obj_unifbuff_create(rsge_obj_unifbuff_t* ub,GLuint binding,char* name,rsge_shaderprg_t* shaderProgs,int shaderProgCount,size_t size)
 * \brief Creates a new uniform buffer object.
 * \param[out] ub The pointer to the uniform buffer object.
 * \param[in] binding The binding value.
 * \param[in] name The name of the uniform buffer object.
 * \param[in] shaderProgs The shader programs to use.
 * \param[in] shaderProgCount The number of shader programs to use.
 * \param[in] size The size of the uniform buffer.
 * \return An error code.
 */
rsge_error_e rsge_obj_unifbuff_create(rsge_obj_unifbuff_t* ub,GLuint binding,char* name,rsge_shaderprg_t* shaderProgs,int shaderProgCount,size_t size);

/**
 * \fn rsge_error_e rsge_obj_unifbuff_destroy(rsge_obj_unifbuff_t* ub)
 * \brief Destroyes the uniform buffer object.
 * \param[out] ub The pointer to the uniform buffer object.
 * \return An error code.
 */
rsge_error_e rsge_obj_unifbuff_destroy(rsge_obj_unifbuff_t* ub);

#endif
