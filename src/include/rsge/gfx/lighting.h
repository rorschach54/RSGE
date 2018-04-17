#ifndef __RSGE_GFX_LIGHTING_H_
#define __RSGE_GFX_LIGHTING_H_ 1

#include <rsge/gfx/gl.h>
#include <rsge/error.h>

#define rsge_lighting_enableLight(i) glEnable(GL_LIGHT##i)
#define rsge_lighting_disabeLight(i) glDisable(GL_LIGHT##i)
#define rsge_lighting_setpos(i,pos) glLightfv(GL_LIGHT##i,GL_POSITION,(const GLfloat*)(pos))
#define rsge_lighting_setambient(i,v) glLightfv(GL_LIGHT##i,GL_AMBIENT,(const GLfloat*)(v))
#define rsge_lighting_setdiffuse(i,v) glLightfv(GL_LIGHT##i,GL_DIFFUSE,(const GLfloat*)(v))

#endif