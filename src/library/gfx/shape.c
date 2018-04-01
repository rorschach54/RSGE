#include <rsge/gfx/shape.h>

rsge_error_e rsge_shape_render(GLenum mode,rsge_shape_vert_t* vertices,int verticeCount) {
	glBegin(mode);
	for(size_t i = 0;i < verticeCount;i++) {
		rsge_shape_vert_t* vert = (rsge_shape_vert_t*)&vertices[i];

		if(vert->flags & RSGE_SHAPE_VERT_FLAG_COLOR) glColor4i(vert->color[0],vert->color[1],vert->color[2],vert->color[3]);
		if(vert->flags & RSGE_SHAPE_VERT_FLAG_FOG) glFogCoordf(vert->fogCoord);
		glVertex3f(vert->pos[0],vert->pos[1],vert->pos[2]);
	}
	glEnd();
	return RSGE_ERROR_NONE;
}
