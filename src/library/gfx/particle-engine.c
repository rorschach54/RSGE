#include <rsge/gfx/particle-engine.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

rsge_error_e rsge_particles_create(rsge_particles_t* particles,rsge_particle_t* p,int pc,rsge_surface_t* surfaces[2]) {
    memset(particles,0,sizeof(rsge_particles_t));
    
    particles->particles = p;
    particles->particleCount = pc;
    particles->minY = -5.0f;
    particles->deceleration = 0.0025f;
    
    if(surfaces[0]->texture == 0) {
        rsge_error_e err = rsge_surface_render(surfaces[0],1.0f,1.0f);
        if(err != RSGE_ERROR_NONE) return err;
	    if(surfaces[0]->list != 0 && !(surfaces[0]->flags & RSGE_SURFACE_FLAG_MIPMAP)) {
		    glDeleteLists(1,surfaces[0]->list);
	    }
    }
    
    if(surfaces[1]->texture == 0) {
        rsge_error_e err = rsge_surface_render(surfaces[1],1.0f,1.0f);
        if(err != RSGE_ERROR_NONE) return err;
	    if(surfaces[1]->list != 0 && !(surfaces[1]->flags & RSGE_SURFACE_FLAG_MIPMAP)) {
		    glDeleteLists(1,surfaces[1]->list);
	    }
    }
    
    particles->textures[0] = surfaces[0]->texture;
    particles->textures[1] = surfaces[1]->texture;
    return RSGE_ERROR_NONE;
}

rsge_error_e rsge_particles_destroy(rsge_particles_t* particles) {
    memset(particles,0,sizeof(rsge_particles_t));
    return RSGE_ERROR_NONE;
}

rsge_error_e rsge_particles_update(rsge_particles_t* particles) {
    for(int i = 0;i < particles->particleCount;i++) {
        rsge_particle_t* particle = (rsge_particle_t*)&particles->particles[i];
        particle->pos[1] += particle->acceleration-particle->deceleration;
        particle->deceleration += particles->deceleration;
        
        particle->pos[0] += particle->movPos[0];
        particle->pos[1] += particle->movPos[1];
        particle->pos[2] += particle->movPos[2];
        
        particle->direction += ((((((int)(0.5-0.1+0.1)*rand() % 11)+1)-1+1)*rand() % 11)+1);
        
        if(particle->pos[1] < particles->minY) {
            particle->pos[1] = particle->pos[2] = particles->minY;
            particle->direction = 0;
            particle->deceleration = particles->deceleration;
        }
    }
    return RSGE_ERROR_NONE;
}

rsge_error_e rsge_particles_render(rsge_particles_t* particles) {
    for(int i = 0;i < particles->particleCount;i++) {
        rsge_particle_t* particle = (rsge_particle_t*)&particles->particles[i];
        
        glPushMatrix();
        glTranslatef(particle->pos[0],particle->pos[1],particle->pos[2]);
        glRotatef(particle->direction-90,0,0,1);
        glScalef(particle->scale,particle->scale,particle->scale);
        
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        
        glBlendFunc(GL_DST_COLOR,GL_ZERO);
	    glBindTexture(GL_TEXTURE_2D,particles->textures[0]);
        
        glBegin(GL_QUADS);
        glTexCoord2d(0,0);
        glVertex3f(-1,-1,0);
        glTexCoord2d(1,0);
        glVertex3f(1,-1,0);
        glTexCoord2d(1,1);
        glVertex3f(1,1,0);
        glTexCoord2d(0,1);
        glVertex3f(-1,1,0);
        glEnd();
        
        glBlendFunc(GL_ONE,GL_ONE);
	    glBindTexture(GL_TEXTURE_2D,particles->textures[1]);
	    
	    glBegin(GL_QUADS);
        glTexCoord2d(0,0);
        glVertex3f(-1,-1,0);
        glTexCoord2d(1,0);
        glVertex3f(1,-1,0);
        glTexCoord2d(1,1);
        glVertex3f(1,1,0);
        glTexCoord2d(0,1);
        glVertex3f(-1,1,0);
        glEnd();
        
        glEnable(GL_DEPTH_TEST);
        
        glPopMatrix();
    }
    return RSGE_ERROR_NONE;
}