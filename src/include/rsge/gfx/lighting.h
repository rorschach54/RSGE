#ifndef __RSGE_GFX_LIGHTING_H_
#define __RSGE_GFX_LIGHTING_H_ 1

/** \file src/include/rsge/gfx/image.h
 * \brief Image stuff
 * \author Spaceboy Ross
 */

#include <rsge/gfx/gl.h>
#include <rsge/error.h>

/**
 * Enables a light.
 */
#define rsge_lighting_enableLight(i) glEnable(GL_LIGHT##i)
/**
 * Disables a light.
 */
#define rsge_lighting_disabeLight(i) glDisable(GL_LIGHT##i)
/**
 * Sets a light position.
 */
#define rsge_lighting_setpos(i,pos) glLightfv(GL_LIGHT##i,GL_POSITION,(const GLfloat*)(pos))
/**
 * Sets a light ambient value.
 */
#define rsge_lighting_setambient(i,v) glLightfv(GL_LIGHT##i,GL_AMBIENT,(const GLfloat*)(v))
/**
 * Sets a light diffuse value.
 */
#define rsge_lighting_setdiffuse(i,v) glLightfv(GL_LIGHT##i,GL_DIFFUSE,(const GLfloat*)(v))

#endif
