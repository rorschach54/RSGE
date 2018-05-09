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
#include <rsge/gfx/uniforms/camera.h>
#include <rsge/gfx/uniforms/material.h>
#include <rsge/gfx/uniforms/object.h>
#include <rsge/gfx/uniforms/point-light.h>
#include <rsge/gfx/uniforms/render.h>
#include <rsge/gfx/uniforms/scene.h>
#include <rsge/gfx/mesh.h>
#include <rsge/gfx/object.h>
#include <rsge/gfx/shader.h>
#include <rsge/error.h>
#include <list.h>
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
        /**
         * \brief Buffered textures
         */
        rsge_obj_texbuff_t* bufferedTexs;
        /**
         * \brief Buffered texture count
         */
        int bufferedTexsCount;
    } texBuffs;
    
    /**
     * \brief The resolutions.
     */
    struct {
        /**
         * \brief Internal.
         */
        vec2 internal;
        
        /**
         * \brief Shadow Map.
         */
        vec2 shadowMap;
        
        /**
         * \brief Output.
         */
        vec2 output;
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
    
    /**
     * \brief Vertex Buffers
     */
    struct {
        /**
         * \brief Primary vertex buffer
         */
        rsge_obj_vert_t primary;
    } vertBuffs;
    
    /**
     * \brief The scale of the resolution.
     */
    float resolutionScale;
} rsge_elglr_t;

/**
 * \fn rsge_error_e rsge_elglr_bufferverts(rsge_elglr_t* elglr,list_t* verts)
 * \brief Buffers vertices
 * \param[out] elglr The instance of ELGLR to use.
 * \param[in] verts The list of vertices to use.
 * \return An error code.
 */
rsge_error_e rsge_elglr_bufferverts(rsge_elglr_t* elglr,list_t* verts);

/**
 * \fn rsge_error_e rsge_elglr_buffertexs(rsge_elglr_t* elglr)
 * \biref Buffers the textures.
 * \param[out] elglr The instance of ELGLR to use.
 * \return An error code.
 */
rsge_error_e rsge_elglr_buffertexs(rsge_elglr_t* elglr);

/**
 * \fn rsge_error_e rsge_elglr_mkvertarray(rsge_elglr_t* elglr,list_t* objs,list_t** vertData)
 * \brief Creates a vertex array/list.
 * \param[out] elglr The instance of ELGLR to use.
 * \param[in] objs The list of objects to use.
 * \param[out] vertData The list that contains the vertex data.
 * \return An error code.
 */
rsge_error_e rsge_elglr_mkvertarray(rsge_elglr_t* elglr,list_t* objs,list_t** vertData);

/**
 * \fn rsge_error_e rsge_elglr_setres(rsge_elglr_t* elglr,vec2 res)
 * \brief Sets the resolution.
 * \param[out] elglr The instance of ELGLR to use.
 * \param[in] res The resolution to use.
 * \return An error code.
 */
rsge_error_e rsge_elglr_setres(rsge_elglr_t* elglr,vec2 res);

/**
 * \fn rsge_error_e rsge_elglr_updateSceneUniforms(rsge_elglr_t* elglr,vec4 ambientLightIntensity)
 * \brief Updates the scene uniforms.
 * \param[out] elglr The instance of ELGLR to use.
 * \param[in] ambientLightIntensity The ambient light intensity.
 * \return An error code.
 */
rsge_error_e rsge_elglr_updateSceneUniforms(rsge_elglr_t* elglr,vec4 ambientLightIntensity);

/**
 * \fn rsge_error_e rsge_elglr_updateCameraUniforms(rsge_elglr_t* elglr,rsge_camera_t* cam)
 * \brief Updates the camera uniforms.
 * \param[out] elglr The instance of ELGLR to use.
 * \param[in] cam The camera to use. Set to NULL to use the main camera.
 * \return An error code.
 */
rsge_error_e rsge_elglr_updateCameraUniforms(rsge_elglr_t* elglr,rsge_camera_t* cam);

/**
 * \fn rsge_error_e rsge_elglr_updateObjectUniforms(rsge_elglr_t* elglr,rsge_object_t* obj)
 * \brief Updates the object uniforms.
 * \param[out] elglr The instance of ELGLR to use.
 * \param[in] obj The object to use.
 * \return An error code.
 */
rsge_error_e rsge_elglr_updateObjectUniforms(rsge_elglr_t* elglr,rsge_object_t* obj);

/**
 * \fn rsge_error_e rsge_elglr_updateMaterialUniforms(rsge_elglr_t* elglr,rsge_material_t* mat)
 * \brief Updates the material uniforms.
 * \param[out] elglr The instance of ELGLR to use.
 * \param[in] mat The material to use.
 * \return An error code.
 */
rsge_error_e rsge_elglr_updateMaterialUniforms(rsge_elglr_t* elglr,rsge_material_t* mat);

/**
 * \fn rsge_error_e rsge_elglr_updatePointLightUniforms(rsge_elglr_t* elglr,rsge_point_light_t* pl)
 * \brief Updates the point light uniforms.
 * \param[out] elglr The instance of ELGLR to use.
 * \param[in] pl The point light to use.
 * \return An error code.
 */
rsge_error_e rsge_elglr_updatePointLightUniforms(rsge_elglr_t* elglr,rsge_point_light_t* pl);

/**
 * \fn rsge_error_e rsge_elglr_applyState(rsge_elglr_t* elglr,rsge_elglr_state_t* state,rsge_obj_fb_t* framebufferOverride)
 * \brief Applies a state.
 * \param[out] elglr The instance of ELGLR to use.
 * \param[in] state The state to use.
 * \param[in] framebufferOverride Overrides the framebuffer that the state uses. Set to NULL to use the state's framebuffer.
 * \return An error code.
 */
rsge_error_e rsge_elglr_applyState(rsge_elglr_t* elglr,rsge_elglr_state_t* state,rsge_obj_fb_t* framebufferOverride);

/**
 * \fn rsge_error_e rsge_elglr_bindTextureArray(rsge_elglr_t* elglr,rsge_obj_texbuff_t* textures,size_t textureCount)
 * \brief Binds an array of textures.
 * \param[out] elglr The instance of ELGLR to use.
 * \param[in] textures The array of textures to use.
 * \param[in] textureCount The number of textures.
 * \return An error code.
 */
rsge_error_e rsge_elglr_bindTextureArray(rsge_elglr_t* elglr,rsge_obj_texbuff_t* textures,size_t textureCount);

/**
 * \fn rsge_error_e rsge_elglr_bindTextureList(rsge_elglr_t* elglr,list_t* textures)
 * \brief Binds a list of textures.
 * \param[out] elglr The instance of ELGLR to use.
 * \param[in] textures The list of textures to use.
 * \return An error code.
 */
rsge_error_e rsge_elglr_bindTextureList(rsge_elglr_t* elglr,list_t* textures);

/**
 * \fn rsge_error_e rsge_elglr_init()
 * \brief Initializes ELGLR. This is automatically done.
 * \return An error code.
 */
rsge_error_e rsge_elglr_init();

/**
 * \fn rsge_error_e rsge_elglr_deinit()
 * \brief Deinitializes ELGLR. This is automatically done.
 * \return An error code.
 */
rsge_error_e rsge_elglr_deinit();

#endif
