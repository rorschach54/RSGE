#ifndef __RSGE_GFX_GL_H_
#define __RSGE_GFX_GL_H_ 1

/** \file src/include/rsge/gfx/gl.h
 * \brief Loads the OpenGL headers.
 * \author Spaceboy Ross
 */

#include <rsge/config.h>

#if CONFIG_OS == android
#elif CONFIG_OS == linux
#if 0
#define GLFW_INCLUDE_VULKAN
#endif
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#else
#error "Unknown platform"CONFIG_PLATFORM
#endif

#include <GL/glu.h>

#endif