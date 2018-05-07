#ifndef __RSGE_GFX_LIGHTING_H_
#define __RSGE_GFX_LIGHTING_H_ 1

/** \file src/include/rsge/gfx/lighting.h
 * \brief Lighting stuff
 * \author Spaceboy Ross
 */

#include <rsge/gfx/colors.h>
#include <rsge/gfx/object.h>
#include <rsge/gfx/gl.h>
#include <rsge/error.h>

/** \struct rsge_point_light_t src/include/rsge/gfx/lighting.h rsge/gfx/lighting.h
 * \brief Point light
 */
typedef struct {
    /**
     * \brief The object that is this light.
     */
    rsge_object_t obj;
    
    /**
     * \brief The insensity of the light.
     */
    rsge_color_rgba_t intensity;
} rsge_point_light_t;

#endif
#ifndef __RSGE_GFX_LIGHTING_H_
#define __RSGE_GFX_LIGHTING_H_ 1

/** \file src/include/rsge/gfx/lighting.h
 * \brief Lighting stuff
 * \author Spaceboy Ross
 */

#include <rsge/gfx/colors.h>
#include <rsge/gfx/object.h>
#include <rsge/gfx/gl.h>
#include <rsge/error.h>

/** \struct rsge_point_light_t src/include/rsge/gfx/lighting.h rsge/gfx/lighting.h
 * \brief Point light
 */
typedef struct {
    /**
     * \brief The object that is this light.
     */
    rsge_object_t obj;
    
    /**
     * \brief The insensity of the light.
     */
    rsge_color_rgba_t intensity;
} rsge_point_light_t;

/**
 * Enables a light. DEPRECATED
 */
#define rsge_lighting_enableLight(i) glEnable(GL_LIGHT##i)
/**
 * Disables a light. DEPRECATED
 */
#define rsge_lighting_disabeLight(i) glDisable(GL_LIGHT##i)
/**
 * Sets a light position. DEPRECATED
 */
#define rsge_lighting_setpos(i,pos) glLightfv(GL_LIGHT##i,GL_POSITION,(const GLfloat*)(pos))
/**
 * Sets a light ambient value. DEPRECATED
 */
#define rsge_lighting_setambient(i,v) glLightfv(GL_LIGHT##i,GL_AMBIENT,(const GLfloat*)(v))
/**
 * Sets a light diffuse value. DEPRECATED
 */
#define rsge_lighting_setdiffuse(i,v) glLightfv(GL_LIGHT##i,GL_DIFFUSE,(const GLfloat*)(v))

#endif
