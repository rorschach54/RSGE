#include <rsge/gfx/font.h>
#include <rsge/config.h>
#include <string.h>

#if CONFIG_USE_FREETYPE == 1
#include <rsge/gfx/font/freetype.h>
#endif

rsge_error_e rsge_font_fromFile(rsge_font_t* font,char* path,size_t sz) {
	memset(font,0,sizeof(rsge_font_t));
	rsge_error_e err;
#if CONFIG_USE_FREETYPE == 1
	err = rsge_font_freetype_fromFile(font,path,sz);
	if(err == RSGE_ERROR_NONE) return RSGE_ERROR_NONE;
#endif
	return RSGE_ERROR_INVALID_FILE;
}
