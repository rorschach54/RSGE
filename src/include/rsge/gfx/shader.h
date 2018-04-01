#ifndef __RSGE_GFX_SHADER_H_
#define __RSGE_GFX_SHADER_H_ 1

#include <rsge/gfx/gl.h>
#include <rsge/error.h>

/**
 * \struct rsge_shader_t rsge/gfx/shader.h
 * \brief Defines a shader.
 */
typedef struct {
	/**
	 * \brief The shader ID.
	 */
	GLuint id;
} rsge_shader_t;

/**
 * \struct rsge_shaderprg_t rsge/gfx/shader.h
 * \brief Defines a shader program.
 */
typedef struct {
	/**
	 * \brief The shader program ID.
	 */
	GLuint id;
} rsge_shaderprg_t;

/**
 * \fn rsge_error_e rsge_shader_create(rsge_shader_t* shader,GLenum type)
 * \brief Creates a new shader with no source code.
 * \param shader The pointer to the shader.
 * \param type The type of shader to create.
 * \return An error code.
 */
rsge_error_e rsge_shader_create(rsge_shader_t* shader,GLenum type);

/**
 * \fn rsge_error_e rsge_shader_destroy(rsge_shader_t* shader)
 * \brief Destroyes the shader.
 * \param shader The pointer to the shader.
 * \return An error code.
 */
rsge_error_e rsge_shader_destroy(rsge_shader_t* shader);

/**
 * \fn rsge_error_e rsge_shader_compile(rsge_shader_t* shader,char* source)
 * \brief Sets the shader source code and compiles.
 * \param shader The pointer to the shader.
 * \param source The source code to use.
 * \return An error code.
 */
rsge_error_e rsge_shader_compile(rsge_shader_t* shader,char* source);

/**
 * \fn rsge_error_e rsge_shaderprg_create(rsge_shaderprg_t* prog)
 * \brief Creates a new shader program.
 * \param prog The pointer to the shader program.
 * \return An error code.
 */
rsge_error_e rsge_shaderprg_create(rsge_shaderprg_t* prog);

/**
 * \fn rsge_error_e rsge_shaderprg_destroy(rsge_shaderprg_t* prog)
 * \brief Destroyes the shader program.
 * \param prog The pointer to the shader program.
 * \return An error code.
 */
rsge_error_e rsge_shaderprg_destroy(rsge_shaderprg_t* prog);

#endif
