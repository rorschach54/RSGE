#ifndef __RSGE_GFX_PARTICLE_ENGINE_H_
#define __RSGE_GFX_PARTICLE_ENGINE_H_ 1

#include <rsge/gfx/gl.h>
#include <rsge/gfx/surface.h>
#include <rsge/error.h>
#include <linmath.h>

typedef struct {
    /**
     * \brief The position.
     */
    vec3 pos;
    
    /**
     * \brief The movement position.
     */
    vec3 movPos;
    
    /**
     * \brief The color of the particle.
     */
    int color[4];
    
    /**
     * \brief The direction of the particle.
     */
    float direction;
    
    /**
     * \brief The acceleration of the particle.
     */
    float acceleration;
    
    /**
     * \brief The deceleration of the particle.
     */
    float deceleration;
    
    /**
     * \brief The scale of the particle.
     */
    float scale;
} rsge_particle_t;

typedef struct {
    /**
     * \brief The particles.
     */
    rsge_particle_t* particles;
    
    /**
     * \brief The number of particles.
     */
    int particleCount;
    
    /**
     * \brief The minmum Y positon to bounce.
     */
    float minY;
    
    /**
     * \brief The ammount to add to the deceleration.
     */
    float deceleration;
    
    /**
     * \brief The textures to use.
     */
    GLuint textures[2];
} rsge_particles_t;

/**
 * \fn rsge_error_e rsge_particles_create(rsge_particles_t* particles,rsge_particle_t* p,int pc,rsge_surface_t* surfaces[2])
 * \brief Creates particles.
 * \param particles The pointer to the particles.
 * \param p The actual particles.
 * \param pc The particle count.
 * \param surfaces The two surfaces to use as the textures.
 * \return An error code.
 */
rsge_error_e rsge_particles_create(rsge_particles_t* particles,rsge_particle_t* p,int pc,rsge_surface_t* surfaces[2]);

/**
 * \fn rsge_error_e rsge_particles_destroy(rsge_particles_t* particles)
 * \brief Destroyes the particles.
 * \param particles The pointer to the particles.
 * \return An error code.
 */
rsge_error_e rsge_particles_destroy(rsge_particles_t* particles);

/**
 * \fn rsge_error_e rsge_particles_update(rsge_particles_t* particles)
 * \brief Calculates and updates the particles.
 * \param particles The pointer to the particles.
 * \return An error code.
 */
rsge_error_e rsge_particles_update(rsge_particles_t* particles);

/**
 * \fn rsge_error_e rsge_particles_render(rsge_particles_t* particles)
 * \brief Renders the particles.
 * \param particles The pointer to the particles.
 * \return An error code.
 */
rsge_error_e rsge_particles_render(rsge_particles_t* particles);

#endif