#include <rsge/assets/rsge_assets.h>
#include <rsge/gfx/shape.h>
#include <string.h>

rsge_error_e rsge_shape_create(rsge_shape_t* shape,GLenum mode,rsge_shape_vert_t* vertices,int verticeCount,rsge_shape_mat_t* materials,int materialCount) {
	memset(shape,0,sizeof(rsge_shape_t));
	
	shape->vertices = vertices;
	shape->verticeCount = verticeCount;
	
	shape->materials = materials;
	shape->materialCount = materialCount;
	
	shape->mode = mode;
	
	shape->list = glGenLists(1);
	if(shape->list == 0) return RSGE_ERROR_OPENGL;
	glNewList(shape->list,GL_COMPILE);
	
	glBegin(shape->mode);
	for(size_t i = 0;i < shape->materialCount;i++) {
		rsge_shape_mat_t* material = &shape->materials[i];
		glMaterialiv(material->face,material->type,(const GLint*)material->values);
	}
	for(size_t i = 0;i < shape->verticeCount;i++) {
		rsge_shape_vert_t* vert = &shape->vertices[i];
		
		if(vert->flags & RSGE_SHAPE_VERT_FLAG_COLOR) glColor4fv((const GLfloat*)vert->color);
		if(vert->flags & RSGE_SHAPE_VERT_FLAG_2NDCOLOR) glSecondaryColor3fv((const GLfloat*)vert->color2nd);
		if(vert->flags & RSGE_SHAPE_VERT_FLAG_FOG) glFogCoordf(vert->fogCoord);
		glVertex3fv((const GLfloat*)vert->pos);
	}
	glEnd();
	
	glEndList();
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_shape_destroy(rsge_shape_t* shape) {
	glDeleteLists(1,shape->list);
	memset(shape,0,sizeof(rsge_shape_t));
	return RSGE_ERROR_NONE;
}


rsge_error_e rsge_shape_render(rsge_shape_t* shape) {
	glPushMatrix();
	glTranslatef(0.0f,0.0f,0.0f);
	glTranslatef(shape->pos[0],shape->pos[1],shape->pos[2]);
	
	glCallList(shape->list);
	
	glPopMatrix();
	return RSGE_ERROR_NONE;
}