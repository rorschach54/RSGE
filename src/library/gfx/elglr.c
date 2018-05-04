#include <rsge/gfx/camera.h>
#include <rsge/gfx/elglr.h>
#include <log.h>
#include <string.h>

rsge_elglr_t rsge_elglr;

rsge_error_e rsge_elglr_buildfbs(rsge_elglr_t* elglr);
rsge_error_e rsge_elglr_mkstates(rsge_elglr_t* elglr);

rsge_error_e rsge_elglr_initshaders(rsge_elglr_t* elglr) {
    log_debug("ELGLR: Initializing shaders");
    
    rsge_error_e err = rsge_obj_unifbuff_create(&elglr->unifBuffs.render,1);
    if(err != RSGE_ERROR_NONE) return err;
    err = rsge_obj_unifbuff_create(&elglr->unifBuffs.scene,2);
    if(err != RSGE_ERROR_NONE) return err;
    err = rsge_obj_unifbuff_create(&elglr->unifBuffs.camera,3);
    if(err != RSGE_ERROR_NONE) return err;
    err = rsge_obj_unifbuff_create(&elglr->unifBuffs.obj,4);
    if(err != RSGE_ERROR_NONE) return err;
    err = rsge_obj_unifbuff_create(&elglr->unifBuffs.mat,5);
    if(err != RSGE_ERROR_NONE) return err;
    err = rsge_obj_unifbuff_create(&elglr->unifBuffs.pointLight,6);
    if(err != RSGE_ERROR_NONE) return err;
    
    err = rsge_elglr_buildfbs(elglr);
    if(err != RSGE_ERROR_NONE) return err;
    
    /* Create texture to screen shader */
    {
        rsge_shader_t shaderVert;
        rsge_error_e err = rsge_shader_fromFile(&shaderVert,GL_VERTEX_SHADER,"rsge@shaders/fullScreenQuad.vert");
        if(err != RSGE_ERROR_NONE) return err;
        
        rsge_shader_t shaderFrag;
        err = rsge_shader_fromFile(&shaderFrag,GL_FRAGMENT_SHADER,"rsge@shaders/copyTexture.frag");
        if(err != RSGE_ERROR_NONE) return err;
        
        err = rsge_shaderprg_create(&elglr->shaderProgs.texToScreen);
        if(err != RSGE_ERROR_NONE) return err;
        
        glAttachShader(elglr->shaderProgs.texToScreen.id,shaderVert.id);
        glAttachShader(elglr->shaderProgs.texToScreen.id,shaderFrag.id);
    }
    
    /* Create make g-buffers shader */
    {
        rsge_shader_t shaderVert;
        rsge_error_e err = rsge_shader_fromFile(&shaderVert,GL_VERTEX_SHADER,"rsge@shaders/transforms.vert");
        if(err != RSGE_ERROR_NONE) return err;
        
        rsge_shader_t shaderFrag;
        err = rsge_shader_fromFile(&shaderFrag,GL_FRAGMENT_SHADER,"rsge@shaders/firstPass.frag");
        if(err != RSGE_ERROR_NONE) return err;
        
        err = rsge_shaderprg_create(&elglr->shaderProgs.makeGBuffs);
        if(err != RSGE_ERROR_NONE) return err;
        
        glAttachShader(elglr->shaderProgs.makeGBuffs.id,shaderVert.id);
        glAttachShader(elglr->shaderProgs.makeGBuffs.id,shaderFrag.id);
    }
    
    /* Create depth pass shader */
    {
        rsge_shader_t shaderVert;
        rsge_error_e err = rsge_shader_fromFile(&shaderVert,GL_VERTEX_SHADER,"rsge@shaders/depthOnly.vert");
        if(err != RSGE_ERROR_NONE) return err;
        
        rsge_shader_t shaderGeom;
        err = rsge_shader_fromFile(&shaderGeom,GL_GEOMETRY_SHADER,"rsge@shaders/depthCube.geom");
        if(err != RSGE_ERROR_NONE) return err;
        
        rsge_shader_t shaderFrag;
        err = rsge_shader_fromFile(&shaderFrag,GL_FRAGMENT_SHADER,"rsge@shaders/depthOnly.frag");
        if(err != RSGE_ERROR_NONE) return err;
        
        err = rsge_shaderprg_create(&elglr->shaderProgs.depthPass);
        if(err != RSGE_ERROR_NONE) return err;
        
        glAttachShader(elglr->shaderProgs.depthPass.id,shaderVert.id);
        glAttachShader(elglr->shaderProgs.depthPass.id,shaderGeom.id);
        glAttachShader(elglr->shaderProgs.depthPass.id,shaderFrag.id);
    }
    
    /* Create lighting pass shader */
    {
        rsge_shader_t shaderVert;
        rsge_error_e err = rsge_shader_fromFile(&shaderVert,GL_VERTEX_SHADER,"rsge@shaders/fullScreenQuad.vert");
        if(err != RSGE_ERROR_NONE) return err;
        
        rsge_shader_t shaderFrag;
        err = rsge_shader_fromFile(&shaderFrag,GL_FRAGMENT_SHADER,"rsge@shaders/lighting.frag");
        if(err != RSGE_ERROR_NONE) return err;
        
        err = rsge_shaderprg_create(&elglr->shaderProgs.lightingPass);
        if(err != RSGE_ERROR_NONE) return err;
        
        glAttachShader(elglr->shaderProgs.lightingPass.id,shaderVert.id);
        glAttachShader(elglr->shaderProgs.lightingPass.id,shaderFrag.id);
    }
    return rsge_elglr_mkstates(elglr);
}

rsge_error_e rsge_elglr_buildfbs(rsge_elglr_t* elglr) {
    log_debug("ELGLR: Building framebuffers");
    
    rsge_error_e err;
    err = rsge_obj_fb_create(&elglr->frameBuffers.firstPass);
    if(err != RSGE_ERROR_NONE) return err;
    glBindFramebuffer(GL_FRAMEBUFFER,elglr->frameBuffers.firstPass.fbo);
    
    GLenum drawBuffers[RSGE_ELGLR_NUMBUFFS];
    for(int i = 0;i < RSGE_ELGLR_NUMBUFFS;i++) {
        err = rsge_obj_texbuff_create(&elglr->texBuffs.firstPassColorBuffers[i],RSGE_OBJ_TEXBUFF_TYPE_COLOR,elglr->resolutions.internalW,elglr->resolutions.internalH,NULL);
        if(err != RSGE_ERROR_NONE) return err;
        glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0+i,GL_TEXTURE_2D,elglr->texBuffs.firstPassColorBuffers[i].tex,0);
        drawBuffers[i] = GL_COLOR_ATTACHMENT0+i;
    }
    glDrawBuffers(RSGE_ELGLR_NUMBUFFS,drawBuffers);
    
    err = rsge_obj_texbuff_create(&elglr->texBuffs.firstPassDepthBuffer,RSGE_OBJ_TEXBUFF_TYPE_DEPTH,elglr->resolutions.internalW,elglr->resolutions.internalH,NULL);
    if(err != RSGE_ERROR_NONE) return err;
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,elglr->texBuffs.firstPassDepthBuffer.tex,0);
    
    err = rsge_obj_fb_create(&elglr->frameBuffers.lightingPass);
    if(err != RSGE_ERROR_NONE) return err;
    glBindFramebuffer(GL_FRAMEBUFFER,elglr->frameBuffers.lightingPass.fbo);
    err = rsge_obj_texbuff_create(&elglr->texBuffs.lightingPassColor,RSGE_OBJ_TEXBUFF_TYPE_COLOR,elglr->resolutions.internalW,elglr->resolutions.internalH,NULL);
    if(err != RSGE_ERROR_NONE) return err;
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,elglr->texBuffs.lightingPassColor.tex,0);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    
    err = rsge_obj_fb_create(&elglr->frameBuffers.depth);
    if(err != RSGE_ERROR_NONE) return err;
    glBindFramebuffer(GL_FRAMEBUFFER,elglr->frameBuffers.depth.fbo);
    err = rsge_obj_texbuff_create(&elglr->texBuffs.depth,RSGE_OBJ_TEXBUFF_TYPE_DEPTHCUBE,elglr->resolutions.shadowMapW,elglr->resolutions.shadowMapH,NULL);
    if(err != RSGE_ERROR_NONE) return err;
    glFramebufferTexture(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,elglr->texBuffs.depth.tex,0);
    return RSGE_ERROR_NONE;
}

rsge_error_e rsge_elglr_mkstates(rsge_elglr_t* elglr) {
    log_debug("ELGLR: Creating states");
    
    elglr->states.buildGBuffs.clearColor = true;
    elglr->states.buildGBuffs.clearDepth = true;
    elglr->states.buildGBuffs.useBlending = false;
    elglr->states.buildGBuffs.useDepthTest = true;
    elglr->states.buildGBuffs.fb = &elglr->frameBuffers.firstPass;
    elglr->states.buildGBuffs.prg = &elglr->shaderProgs.makeGBuffs;
    
    elglr->states.texToScreen.clearColor = true;
    elglr->states.texToScreen.clearDepth = false;
    elglr->states.texToScreen.useBlending = true;
    elglr->states.texToScreen.useDepthTest = false;
    elglr->states.texToScreen.fb = NULL;
    elglr->states.texToScreen.prg = &elglr->shaderProgs.texToScreen;
    
    elglr->states.depthPass.clearColor = false;
    elglr->states.depthPass.clearDepth = true;
    elglr->states.depthPass.useBlending = true;
    elglr->states.depthPass.useDepthTest = true;
    elglr->states.depthPass.fb = &elglr->frameBuffers.depth;
    elglr->states.depthPass.prg = &elglr->shaderProgs.depthPass;
    
    elglr->states.lightingPass.clearColor = false;
    elglr->states.lightingPass.clearDepth = false;
    elglr->states.lightingPass.useBlending = true;
    elglr->states.lightingPass.useDepthTest = false;
    elglr->states.lightingPass.fb = &elglr->frameBuffers.lightingPass;
    elglr->states.lightingPass.prg = &elglr->shaderProgs.lightingPass;
    return RSGE_ERROR_NONE;
}

rsge_error_e rsge_elglr_init() {
    rsge_error_e err;
	rsge_camera_t* cam;
	err = rsge_camera_getmaincam(&cam);
	if(err != RSGE_ERROR_NONE) return err;
	
	err = rsge_camera_create(cam);
	if(err != RSGE_ERROR_NONE) return err;
	
	memset(&rsge_elglr,0,sizeof(rsge_elglr_t));
	rsge_elglr.resolutions.internalW = rsge_elglr.resolutions.internalH = 1;
	rsge_elglr.resolutions.shadowMapW = rsge_elglr.resolutions.shadowMapH = 1;
    return RSGE_ERROR_NONE;
}

rsge_error_e rsge_elglr_deinit() {
    return RSGE_ERROR_NONE;
}