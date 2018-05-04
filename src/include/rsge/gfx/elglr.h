#ifndef __RSGE_GFX_ELGLR_H_
#define __RSGE_GFX_ELGLR_H_ 1

/** \file src/include/rsge/gfx/elglr.h
 * \brief ELGLR - Enhanced Linear GL Renderer
 * \author Spaceboy Ross
 */

#include <rsge/gfx/objects/fb.h>
#include <rsge/gfx/objects/texbuff.h>
#include <rsge/gfx/objects/unifbuff.h>
#include <rsge/gfx/objects/vert.h>
#include <rsge/gfx/shader.h>
#include <rsge/error.h>
#include <stdbool.h>

#define RSGE_ELGLR_NUMBUFFS 4

/** \struct rsge_elglr_state_t src/include/rsge/gfx/elglr.h rsge/gfx/elglr.h
 * \brief ELGLR State
 */
typedef struct {
    /**
     * \brief Clear color
     */
    bool clearColor;
    /**
     * \brief Clear depth
     */
    bool clearDepth;
    /**
     * \brief Use blending
     */
    bool useBlending;
    /**
     * \brief Use depth test
     */
    bool useDepthTest;
    /**
     * \brief The framebuffer of the state.
     */
    rsge_obj_fb_t* fb;
    /**
     * \brief The shader program of the state.
     */
    rsge_shaderprg_t* prg;
} rsge_elglr_state_t;

/** \struct rsge_elglr_t src/include/rsge/gfx/elglr.h rsge/gfx/elglr.h
 * \brief ELGLR
 */
typedef struct {
    /**
     * \brief The uniform buffers.
     */
    struct {
        /**
         * \brief The render uniform buffer.
         */
        rsge_obj_unifbuff_t render;
        /**
         * \brief The scene uniform buffer.
         */
        rsge_obj_unifbuff_t scene;
        /**
         * \brief The camera uniform buffer.
         */
        rsge_obj_unifbuff_t camera;
        /**
         * \brief The object uniform buffer.
         */
        rsge_obj_unifbuff_t obj;
        /**
         * \brief The material uniform buffer.
         */
        rsge_obj_unifbuff_t mat;
        /**
         * \brief The point light uniform buffer.
         */
        rsge_obj_unifbuff_t pointLight;
    } unifBuffs;
    
    /**
     * \brief The shader programs.
     */
    struct {
        /**
         * \brief The texture to screen shader program.
         */
        rsge_shaderprg_t texToScreen;
        /**
         * \brief The make g-buffers shader program.
         */
        rsge_shaderprg_t makeGBuffs;
        /**
         * \brief The depth pass shader program.
         */
        rsge_shaderprg_t depthPass;
        /**
         * \brief The lighting pass shader program.
         */
        rsge_shaderprg_t lightingPass;
    } shaderProgs;
    
    /**
     * \brief The framebuffers.
     */
    struct {
        /**
         * \brief The first pass framebuffer.
         */
        rsge_obj_fb_t firstPass;
        /**
         * \brief The lighting pass framebuffer.
         */
        rsge_obj_fb_t lightingPass;
        /**
         * \brief The depth framebuffer.
         */
        rsge_obj_fb_t depth;
    } frameBuffers;
    
    /**
     * \brief The texture buffers.
     */
    struct {
        /**
         * \brief The first pass color texture buffers.
         */
        rsge_obj_texbuff_t firstPassColorBuffers[RSGE_ELGLR_NUMBUFFS];
        /**
         * \brief The first pass depth texture buffer.
         */
        rsge_obj_texbuff_t firstPassDepthBuffer;
        /**
         * \brief The lighting pass color texture buffer.
         */
        rsge_obj_texbuff_t lightingPassColor;
        /**
         * \brief The depth texture buffer.
         */
        rsge_obj_texbuff_t depth;
    } texBuffs;
    
    /**
     * \brief The resolutions.
     */
    struct {
        /**
         * \brief Internal Width.
         */
        int internalW;
        /**
         * \brief Internal Height.
         */
        int internalH;
        
        /**
         * \brief Shadow Map Width.
         */
        int shadowMapW;
        /**
         * \brief Shadow Map Height.
         */
        int shadowMapH;
    } resolutions;
    
    /**
     * \brief The states of ELGLR.
     */
    struct {
        /**
         * \brief Build Graphics Buffers state.
         */
        rsge_elglr_state_t buildGBuffs;
        /**
         * \brief Texture to screen state.
         */
        rsge_elglr_state_t texToScreen;
        /**
         * \brief Depth pass state.
         */
        rsge_elglr_state_t depthPass;
        /**
         * \brief lighing pass state.
         */
        rsge_elglr_state_t lightingPass;
    } states;
} rsge_elglr_t;

/**
 * \fn rsge_error_e rsge_elglr_init()
 * \brief Initializes ELGLR. This is automatically done.
 */
rsge_error_e rsge_elglr_init();

/**
 * \fn rsge_error_e rsge_elglr_deinit()
 * \brief Deinitializes ELGLR. This is automatically done.
 */
rsge_error_e rsge_elglr_deinit();

#endif