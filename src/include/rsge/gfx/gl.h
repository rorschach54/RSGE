#ifndef __RSGE_GFX_GL_H_
#define __RSGE_GFX_GL_H_ 1

#include <rsge/config.h>

#if CONFIG_PLATFORM == android
#elif CONFIG_PLATFORM == linux
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#else
#error "Unknown platform"CONFIG_PLATFORM
#endif

#include <GL/glu.h>

#endif