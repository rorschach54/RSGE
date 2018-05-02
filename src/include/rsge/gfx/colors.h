#ifndef __RSGE_GFX_COLORS_H_
#define __RSGE_GFX_COLORS_H_ 1

/** \file src/include/rsge/gfx/colors.h
 * \brief Just a bunch of macro's that defines colors
 * \author Spaceboy Ross
 */

#define RSGE_COLOR_RGBA(r,g,b,a) ((int[4]){r,g,b,a})
#define RSGE_COLOR_RGB(r,g,b) RSGE_COLOR_RGBA(r,g,b,0xFF)

#define RSGE_COLOR_BLACK RSGE_COLOR_RGB(0x00,0x00,0x00)
#define RSGE_COLOR_RED RSGE_COLOR_RGB(0xFF,0x00,0x00)
#define RSGE_COLOR_GREEN RSGE_COLOR_RGB(0x00,0xFF,0x00)
#define RSGE_COLOR_BLUE RSGE_COLOR_RGB(0x00,0x00,0xFF)
#define RSGE_COLOR_WHITE RSGE_COLOR_RGB(0xFF,0xFF,0xFF)

#endif
