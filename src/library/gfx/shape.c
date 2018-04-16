#include <rsge/gfx/shape.h>
#include <string.h>

rsge_error_e rsge_shape_create(rsge_shape_t* shape,GLenum mode,rsge_shape_vert_t* vertices,int verticeCount,uint8_t flags) {
	memset(shape,0,sizeof(rsge_shape_t));

	shape->flags = flags;
	shape->mode = mode;
	shape->vertices = vertices;
	shape->verticeCount = verticeCount;

	if((shape->flags & RSGE_SHAPE_FLAG_MULTIDISPLLST) != RSGE_SHAPE_FLAG_MULTIDISPLLST) {
		/* Creates the list */
		shape->list = glGenLists(1);
		glNewList(shape->list,GL_COMPILE);

		glPushMatrix();
		glBegin(mode);
	
		for(size_t i = 0;i < verticeCount;i++) {
			rsge_shape_vert_t* vert = (rsge_shape_vert_t*)&vertices[i];
	
			// TODO: add materials
			if(vert->flags & RSGE_SHAPE_VERT_FLAG_COLOR) glColor4f(vert->color[0],vert->color[1],vert->color[2],vert->color[3]);
			if(vert->flags & RSGE_SHAPE_VERT_FLAG_FOG) glFogCoordf(vert->fogCoord);
			glVertex3f(vert->pos[0],vert->pos[1],vert->pos[2]);
		}
	
		glEnd();
		glPopMatrix();
		glEndList();
	}
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_shape_destroy(rsge_shape_t* shape) {
	if((shape->flags & RSGE_SHAPE_FLAG_MULTIDISPLLST) != RSGE_SHAPE_FLAG_MULTIDISPLLST) {
		glDeleteLists(1,shape->list);
	}
	memset(shape,0,sizeof(rsge_shape_t));
	return RSGE_ERROR_NONE;
}


rsge_error_e rsge_shape_render(rsge_shape_t* shape) {
	glPushMatrix();
	glTranslatef(shape->pos[0],shape->pos[1],shape->pos[2]);
	if((shape->flags & RSGE_SHAPE_FLAG_MULTIDISPLLST) != RSGE_SHAPE_FLAG_MULTIDISPLLST) glCallList(shape->list);
	else {

		glBegin(shape->mode);
	
		for(size_t i = 0;i < shape->verticeCount;i++) {
			rsge_shape_vert_t* vert = (rsge_shape_vert_t*)&shape->vertices[i];
	
			// TODO: add materials
			if(vert->flags & RSGE_SHAPE_VERT_FLAG_COLOR) glColor4f(vert->color[0],vert->color[1],vert->color[2],vert->color[3]);
			if(vert->flags & RSGE_SHAPE_VERT_FLAG_FOG) glFogCoordf(vert->fogCoord);
			glVertex3f(vert->pos[0],vert->pos[1],vert->pos[2]);
		}
	
		glEnd();
	}
	glPopMatrix();
	return RSGE_ERROR_NONE;
}
