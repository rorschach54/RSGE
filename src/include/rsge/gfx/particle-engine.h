#ifndef __RSGE_GFX_PARTICLE_ENGINE_H_
#define __RSGE_GFX_PARTICLE_ENGINE_H_ 1

/** \file src/include/rsge/gfx/particle-engine.h
 * \brief Particle stuff
 * \author Spaceboy Ross
 */

#include <rsge/gfx/gl.h>
#include <rsge/gfx/surface.h>
#include <rsge/error.h>
#include <linmath.h>

/** \struct rsge_particle_t src/include/rsge/gfx/particle-engine.h rsge/gfx/particle-engine.h
 * \brief A single particle
 */
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

/** \struct rsge_particles_t src/include/rsge/gfx/particle-engine.h rsge/gfx/particle-engine.h
 * \brief A bunch of particles
 */
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
 * \param[out] particles The pointer to the particles.
 * \param[in] p The actual particles.
 * \param[in] pc The particle count.
 * \param[in] surfaces The two surfaces to use as the textures.
 * \return An error code.
 */
rsge_error_e rsge_particles_create(rsge_particles_t* particles,rsge_particle_t* p,int pc,rsge_surface_t* surfaces[2]);

/**
 * \fn rsge_error_e rsge_particles_destroy(rsge_particles_t* particles)
 * \brief Destroyes the particles.
 * \param[out] particles The pointer to the particles.
 * \return An error code.
 */
rsge_error_e rsge_particles_destroy(rsge_particles_t* particles);

/**
 * \fn rsge_error_e rsge_particles_update(rsge_particles_t* particles)
 * \brief Calculates and updates the particles.
 * \param[out] particles The pointer to the particles.
 * \return An error code.
 */
rsge_error_e rsge_particles_update(rsge_particles_t* particles);

/**
 * \fn rsge_error_e rsge_particles_render(rsge_particles_t* particles)
 * \brief Renders the particles.
 * \param[out] particles The pointer to the particles.
 * \return An error code.
 */
rsge_error_e rsge_particles_render(rsge_particles_t* particles);

#endif
