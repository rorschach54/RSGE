#include <rsge/gfx/terrain.h>
#include <log.h>
#include <stdlib.h>

rsge_error_e rsge_terrain_create(rsge_terrain_t* terrain,rsge_terrain_point_t** points,int length,int width,rsge_surface_t** surfaces,int surfaceCount) {
    memset(terrain,0,sizeof(rsge_terrain_t));
    
    terrain->width = width;
    terrain->length = length;
    terrain->textureCount = surfaceCount;
    terrain->points = points;
    terrain->textures = malloc(sizeof(GLuint)*terrain->textureCount);
    if(!terrain->textures) {
        log_error("Failed to allocate %d bytes of memory",sizeof(GLuint)*terrain->textureCount);
        return RSGE_ERROR_MALLOC;
    }
    
    for(int i = 0;i < terrain->textureCount;i++) {
        rsge_surface_t* surface = surfaces[i];
        if(surface->texture == 0) {
            rsge_error_e err = rsge_surface_render(surface,1.0f,1.0f);
            if(err != RSGE_ERROR_NONE) {
                free(terrain->textures);
                return err;
            }
        }
	    if(surface->list != 0 && !(surface->flags & RSGE_SURFACE_FLAG_MIPMAP)) {
		    glDeleteLists(1,surface->list);
	    }
        terrain->textures[i] = surface->texture;
    }
    return RSGE_ERROR_NONE;
}

rsge_error_e rsge_terrain_destroy(rsge_terrain_t* terrain) {
    if(terrain->list != 0) glDeleteLists(1,terrain->list);
    free(terrain->textures);
    memset(terrain,0,sizeof(rsge_terrain_t));
    return RSGE_ERROR_NONE;
}

rsge_error_e rsge_terrain_render(rsge_terrain_t* terrain) {
    if(terrain->list == 0) {
        terrain->list = glGenLists(1);
		glNewList(terrain->list,GL_COMPILE);
		glPushMatrix();
		glTranslatef(terrain->pos[0],terrain->pos[1],terrain->pos[2]);
		glBegin(GL_TRIANGLE_STRIP);
		for(int z = 0;z < terrain->length-1;z++) {
		    for(int x = 0;x < terrain->width-1;x++) {
		        rsge_terrain_point_t* point = (rsge_terrain_point_t*)&terrain->points[x][z];
		        rsge_terrain_point_t* xpoint = (rsge_terrain_point_t*)&terrain->points[x+1][z];
		        rsge_terrain_point_t* zpoint = (rsge_terrain_point_t*)&terrain->points[x][z+1];
		        rsge_terrain_point_t* xzpoint = (rsge_terrain_point_t*)&terrain->points[x+1][z+1];
		        if(point->texture-1 > 0 && point->texture-1 < terrain->textureCount) {
                    glBindTexture(GL_TEXTURE_2D,terrain->textures[point->texture-1]);
                    
                    glColor3f(point->data[1]/255.0f,point->data[1]/255.0f,point->data[1]/255.0f);
                    glTexCoord2d(0.0,0.0);
                    glVertex3f(point->data[0],point->data[1],point->data[2]);
                    
                    glTexCoord2f(1.0f,0.0f);
                    glColor3f(xpoint->data[1]/255.0f,xpoint->data[1]/255.0f,xpoint->data[1]/255.0f);
                    glVertex3f(xpoint->data[0],xpoint->data[1],xpoint->data[2]);
                    
                    glTexCoord2f(0.0f,1.0f);
                    glColor3f(zpoint->data[1]/255.0f,zpoint->data[1]/255.0f,zpoint->data[1]/255.0f);
                    glVertex3f(zpoint->data[0],zpoint->data[1],zpoint->data[2]);
                    
                    glColor3f(xzpoint->data[1]/255.0f,xzpoint->data[1]/255.0f,xzpoint->data[1]/255.0f);
                    glTexCoord2f(1.0f,1.0f);
                    glVertex3f(xzpoint->data[0],xzpoint->data[1],xzpoint->data[2]);
		        }
		    }
		}
		glEnd();
        glPopMatrix();
		glEndList();
    }
    glPushMatrix();
	glCallList(terrain->list);
	glPopMatrix();
    return RSGE_ERROR_NONE;
}