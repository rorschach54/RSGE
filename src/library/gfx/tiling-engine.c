#include <rsge/gfx/tiling-engine.h>
#include <stdlib.h>
#include <string.h>

rsge_error_e rsge_tiling_create(rsge_tiling_t* tiling,int** map,int width,int height,rsge_surface_t** surfaces,int surfaceCount,int flags) {
    memset(tiling,0,sizeof(rsge_tiling_t));
    
	tiling->flags = flags;
    tiling->textureCount = surfaceCount;
    tiling->textures = malloc(sizeof(GLuint)*tiling->textureCount);
    if(!tiling->textures) return RSGE_ERROR_MALLOC;
    
    for(int i = 0;i < tiling->textureCount;i++) {
        rsge_surface_t* surface = surfaces[i];
        if(surface->texture == 0) {
            rsge_error_e err = rsge_surface_render(surface,1.0f,1.0f);
            if(err != RSGE_ERROR_NONE) {
                free(tiling->textures);
                return err;
            }
        }
	    if(surface->list != 0 && !(surface->flags & RSGE_SURFACE_FLAG_MIPMAP)) {
		    glDeleteLists(1,surface->list);
	    }
	    tiling->textures[i] = surface->texture;
    }
    
    tiling->width = width;
    tiling->height = height;
    tiling->map = map;
    return RSGE_ERROR_NONE;
}

rsge_error_e rsge_tiling_destroy(rsge_tiling_t* tiling) {
    free(tiling->textures);
    memset(tiling,0,sizeof(rsge_tiling_t));
    return RSGE_ERROR_NONE;
}

rsge_error_e rsge_tiling_render(rsge_tiling_t* tiling) {
    for(int z = 0;z < tiling->height;z++) {
        for(int x = 0;x < tiling->width;x++) {
            int tile = tiling->map[z][x];
            if(tile > 0 && tile-1 < tiling->textureCount && tile-1 >= 0) {
                glBindTexture(GL_TEXTURE_2D,tiling->textures[tile-1]);
                
                glPushMatrix();
				if(tiling->flags & RSGE_TILING_FLAG_TOPDOWN) glTranslatef(tiling->pos[0]+(x*1.0f),tiling->pos[1]+(z*1.0f),tiling->pos[2]);
                else glTranslatef(tiling->pos[0]+(x*1.0f),tiling->pos[1],tiling->pos[2]+(z*1.0f));
                
                glBegin(GL_QUADS);
                
                glTexCoord2d(0.0,0.0);
                glVertex3f(0.0,0.0,0.0);
                
                glTexCoord2d(1.0,0.0);
                glVertex3f(1.0,0.0,0.0);

                glTexCoord2d(1.0,1.0);
                glVertex3f(1.0,1.0,0.0);

                glTexCoord2d(0.0,1.0);
                glVertex3f(0.0,1.0,0.0);
                glEnd();
                
                glPopMatrix();
            }
        }
    }
    return RSGE_ERROR_NONE;
}
