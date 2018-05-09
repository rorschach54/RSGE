#include <rsge/gfx/camera.h>
#include <rsge/gfx/elglr.h>
#include <rsge/settings.h>
#include <log.h>
#include <list.h>
#include <string.h>

extern list_t* rsge_obj_textures_loaded;

rsge_elglr_t rsge_elglr;

rsge_error_e rsge_elglr_buildfbs(rsge_elglr_t* elglr);
rsge_error_e rsge_elglr_mkstates(rsge_elglr_t* elglr);

rsge_error_e rsge_elglr_initshaders(rsge_elglr_t* elglr) {
    log_debug("ELGLR: Initializing shaders");
    
    /* Create texture to screen shader */
    log_debug("ELGLR: Creating texture to screen shader");
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
    log_debug("ELGLR: Creating make g-buffers shader");
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
    log_debug("ELGLR: Creating depth pass shader");
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
    log_debug("ELGLR: Creating lighting pass shader");
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
    
    log_debug("ELGLR: Creating uniform buffers");
    rsge_error_e err = rsge_obj_unifbuff_create(&elglr->unifBuffs.render,1,"RenderUniforms",(rsge_shaderprg_t[4]){
		elglr->shaderProgs.texToScreen,
		elglr->shaderProgs.makeGBuffs,
		elglr->shaderProgs.depthPass,
		elglr->shaderProgs.lightingPass
	},4,sizeof(rsge_unif_render_t));
    if(err != RSGE_ERROR_NONE) return err;
    err = rsge_obj_unifbuff_create(&elglr->unifBuffs.scene,2,"SceneUniforms",(rsge_shaderprg_t[4]){
		elglr->shaderProgs.texToScreen,
		elglr->shaderProgs.makeGBuffs,
		elglr->shaderProgs.depthPass,
		elglr->shaderProgs.lightingPass
	},4,sizeof(rsge_unif_scene_t));
    if(err != RSGE_ERROR_NONE) return err;
    err = rsge_obj_unifbuff_create(&elglr->unifBuffs.camera,3,"CameraUniforms",(rsge_shaderprg_t[4]){
		elglr->shaderProgs.texToScreen,
		elglr->shaderProgs.makeGBuffs,
		elglr->shaderProgs.depthPass,
		elglr->shaderProgs.lightingPass
	},4,sizeof(rsge_unif_cam_t));
    if(err != RSGE_ERROR_NONE) return err;
    err = rsge_obj_unifbuff_create(&elglr->unifBuffs.obj,4,"ObjectUniforms",(rsge_shaderprg_t[4]){
		elglr->shaderProgs.texToScreen,
		elglr->shaderProgs.makeGBuffs,
		elglr->shaderProgs.depthPass,
		elglr->shaderProgs.lightingPass
	},4,sizeof(rsge_unif_obj_t));
    if(err != RSGE_ERROR_NONE) return err;
    err = rsge_obj_unifbuff_create(&elglr->unifBuffs.mat,5,"MaterialUniforms",(rsge_shaderprg_t[4]){
		elglr->shaderProgs.texToScreen,
		elglr->shaderProgs.makeGBuffs,
		elglr->shaderProgs.depthPass,
		elglr->shaderProgs.lightingPass
	},4,sizeof(rsge_unif_material_t));
    if(err != RSGE_ERROR_NONE) return err;
    err = rsge_obj_unifbuff_create(&elglr->unifBuffs.pointLight,6,"PointLightUniforms",(rsge_shaderprg_t[4]){
		elglr->shaderProgs.texToScreen,
		elglr->shaderProgs.makeGBuffs,
		elglr->shaderProgs.depthPass,
		elglr->shaderProgs.lightingPass
	},4,sizeof(rsge_unif_point_light_t));
    if(err != RSGE_ERROR_NONE) return err;
    
    err = rsge_elglr_buildfbs(elglr);
    if(err != RSGE_ERROR_NONE) return err;
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
        err = rsge_obj_texbuff_create(&elglr->texBuffs.firstPassColorBuffers[i],RSGE_OBJ_TEXBUFF_TYPE_COLOR,elglr->resolutions.internal[0],elglr->resolutions.internal[1],NULL);
        if(err != RSGE_ERROR_NONE) return err;
        glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0+i,GL_TEXTURE_2D,elglr->texBuffs.firstPassColorBuffers[i].tex,0);
        drawBuffers[i] = GL_COLOR_ATTACHMENT0+i;
    }
    glDrawBuffers(RSGE_ELGLR_NUMBUFFS,drawBuffers);
    
    err = rsge_obj_texbuff_create(&elglr->texBuffs.firstPassDepthBuffer,RSGE_OBJ_TEXBUFF_TYPE_DEPTH,elglr->resolutions.internal[0],elglr->resolutions.internal[1],NULL);
    if(err != RSGE_ERROR_NONE) return err;
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,elglr->texBuffs.firstPassDepthBuffer.tex,0);
    
    err = rsge_obj_fb_create(&elglr->frameBuffers.lightingPass);
    if(err != RSGE_ERROR_NONE) return err;
    glBindFramebuffer(GL_FRAMEBUFFER,elglr->frameBuffers.lightingPass.fbo);
    err = rsge_obj_texbuff_create(&elglr->texBuffs.lightingPassColor,RSGE_OBJ_TEXBUFF_TYPE_COLOR,elglr->resolutions.internal[0],elglr->resolutions.internal[1],NULL);
    if(err != RSGE_ERROR_NONE) return err;
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,elglr->texBuffs.lightingPassColor.tex,0);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    
    err = rsge_obj_fb_create(&elglr->frameBuffers.depth);
    if(err != RSGE_ERROR_NONE) return err;
    glBindFramebuffer(GL_FRAMEBUFFER,elglr->frameBuffers.depth.fbo);
    err = rsge_obj_texbuff_create(&elglr->texBuffs.depth,RSGE_OBJ_TEXBUFF_TYPE_DEPTHCUBE,elglr->resolutions.shadowMap[0],elglr->resolutions.shadowMap[1],NULL);
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

rsge_error_e rsge_elglr_bufferverts(rsge_elglr_t* elglr,list_t* vertsList) {
    size_t vertCount = 0;
    list_node_t* node;
    list_iterator_t* it = list_iterator_new(vertsList,LIST_HEAD);
    if(!it) return RSGE_ERROR_MALLOC;
    while((node = list_iterator_next(it))) vertCount++;
    list_iterator_destroy(it);
    rsge_vertex_t* verts = malloc(sizeof(rsge_vertex_t)*vertCount);
    if(!verts) return RSGE_ERROR_MALLOC;
    it = list_iterator_new(vertsList,LIST_HEAD);
    if(!it) return RSGE_ERROR_MALLOC;
    size_t vertIndex = 0;
    while((node = list_iterator_next(it))) {
        memcpy(&verts[vertIndex++],(rsge_vertex_t*)node->val,sizeof(rsge_vertex_t));
    }
    list_iterator_destroy(it);
    return rsge_obj_vert_create(&elglr->vertBuffs.primary,verts,vertCount);
}

rsge_error_e rsge_elglr_buffertexs(rsge_elglr_t* elglr) {
    list_iterator_t* it = list_iterator_new(rsge_obj_textures_loaded,LIST_HEAD);
    if(!it) return RSGE_ERROR_MALLOC;
    list_node_t* node;
    while((node = list_iterator_next(it))) {
        rsge_obj_texture_t* texture = (rsge_obj_texture_t*)node->val;
        size_t newBufferedTexCount = elglr->texBuffs.bufferedTexsCount+(texture->id-elglr->texBuffs.bufferedTexsCount);
        if(elglr->texBuffs.bufferedTexsCount == 0) {
            elglr->texBuffs.bufferedTexs = malloc(sizeof(rsge_obj_texbuff_t)*newBufferedTexCount);
            if(elglr->texBuffs.bufferedTexs == NULL) return RSGE_ERROR_MALLOC;
            memset(elglr->texBuffs.bufferedTexs,0,sizeof(rsge_obj_texbuff_t)*newBufferedTexCount);
        }
        if(texture->id > elglr->texBuffs.bufferedTexsCount) {
            elglr->texBuffs.bufferedTexs = realloc(elglr->texBuffs.bufferedTexs,sizeof(rsge_obj_texbuff_t)*newBufferedTexCount);
            if(elglr->texBuffs.bufferedTexs == NULL) return RSGE_ERROR_MALLOC;
            memset(&elglr->texBuffs.bufferedTexs[texture->id],0,sizeof(rsge_obj_texbuff_t));
        }
        elglr->texBuffs.bufferedTexsCount = newBufferedTexCount;
        rsge_error_e err = rsge_obj_texbuff_create(&elglr->texBuffs.bufferedTexs[texture->id],RSGE_OBJ_TEXBUFF_TYPE_FIXEDTEXTURE,texture->width,texture->height,texture);
        if(err != RSGE_ERROR_NONE) return err;
    }
    list_iterator_destroy(it);
    return RSGE_ERROR_NONE;
}

rsge_error_e rsge_elglr_mkvertarray(rsge_elglr_t* elglr,list_t* objs,list_t** vertData) {
    *vertData = list_new();
    if(!vertData) return RSGE_ERROR_MALLOC;
    rsge_vertex_t tempVerts[3];
    list_node_t* node_obj;
    list_iterator_t* it_objs = list_iterator_new(objs,LIST_HEAD);
    if(!it_objs) return RSGE_ERROR_MALLOC;
    while((node_obj = list_iterator_next(it_objs))) {
        rsge_object_t* obj = (rsge_object_t*)node_obj->val;
        list_iterator_t* it_meshes = list_iterator_new(obj->meshes,LIST_HEAD);
        if(!it_meshes) continue;
        list_node_t* node_mesh;
        while((node_mesh = list_iterator_next(it_meshes))) {
            rsge_mesh_t* mesh = (rsge_mesh_t*)node_mesh->val;
            list_iterator_t* it_tri = list_iterator_new(mesh->triangles,LIST_HEAD);
            if(!it_tri) continue;
            list_node_t* node_tri;
            while((node_tri = list_iterator_next(it_tri))) {
                rsge_triangle_t* tri = (rsge_triangle_t*)node_tri->val;
                
                vec3* v1 = (vec3*)(list_at(tri->parent.vertices,tri->vertIndex1)->val);
                vec3* n1 = (vec3*)(list_at(tri->parent.normals,tri->vertIndex1)->val);
                tempVerts[0].pos[0] = *v1[0];
                tempVerts[0].pos[1] = *v1[1];
                tempVerts[0].pos[2] = *v1[2];
                
                tempVerts[0].normal[0] = *n1[0];
                tempVerts[0].normal[1] = *n1[1];
                tempVerts[0].normal[2] = *n1[2];
                
                tempVerts[0].uv[0] = tri->uv1[0];
                tempVerts[0].uv[1] = tri->uv1[1];
                tempVerts[0].uv[2] = tri->uv1[2];
                
                tempVerts[0].tangent[0] = tri->tangent[0];
                tempVerts[0].tangent[1] = tri->tangent[1];
                tempVerts[0].tangent[2] = tri->tangent[2];
                
                tempVerts[0].binormal[0] = tri->binormal[0];
                tempVerts[0].binormal[1] = tri->binormal[1];
                tempVerts[0].binormal[2] = tri->binormal[2];
                
                vec3* v2 = (vec3*)(list_at(tri->parent.vertices,tri->vertIndex2)->val);
                vec3* n2 = (vec3*)(list_at(tri->parent.normals,tri->vertIndex2)->val);
                tempVerts[1].pos[0] = *v2[0];
                tempVerts[1].pos[1] = *v2[1];
                tempVerts[1].pos[2] = *v2[2];
                
                tempVerts[1].normal[0] = *n2[0];
                tempVerts[1].normal[1] = *n2[1];
                tempVerts[1].normal[2] = *n2[2];
                
                tempVerts[1].uv[0] = tri->uv2[0];
                tempVerts[1].uv[1] = tri->uv2[1];
                tempVerts[1].uv[2] = tri->uv2[2];
                
                tempVerts[1].tangent[0] = tri->tangent[0];
                tempVerts[1].tangent[1] = tri->tangent[1];
                tempVerts[1].tangent[2] = tri->tangent[2];
                
                tempVerts[1].binormal[0] = tri->binormal[0];
                tempVerts[1].binormal[1] = tri->binormal[1];
                tempVerts[1].binormal[2] = tri->binormal[2];
                
                vec3* v3 = (vec3*)(list_at(tri->parent.vertices,tri->vertIndex3)->val);
                vec3* n3 = (vec3*)(list_at(tri->parent.normals,tri->vertIndex3)->val);
                tempVerts[2].pos[0] = *v3[0];
                tempVerts[2].pos[1] = *v3[1];
                tempVerts[2].pos[2] = *v3[2];
                
                tempVerts[2].normal[0] = *n3[0];
                tempVerts[2].normal[1] = *n3[1];
                tempVerts[2].normal[2] = *n3[2];
                
                tempVerts[2].uv[0] = tri->uv3[0];
                tempVerts[2].uv[1] = tri->uv3[1];
                tempVerts[2].uv[2] = tri->uv3[2];
                
                tempVerts[2].tangent[0] = tri->tangent[0];
                tempVerts[2].tangent[1] = tri->tangent[1];
                tempVerts[2].tangent[2] = tri->tangent[2];
                
                tempVerts[2].binormal[0] = tri->binormal[0];
                tempVerts[2].binormal[1] = tri->binormal[1];
                tempVerts[2].binormal[2] = tri->binormal[2];
                
                list_rpush(*vertData,list_node_new(&tempVerts[0]));
                list_rpush(*vertData,list_node_new(&tempVerts[3]));
            }
            list_iterator_destroy(it_tri);
        }
        list_iterator_destroy(it_meshes);
    }
    list_iterator_destroy(it_objs);
    return RSGE_ERROR_NONE;
}

rsge_error_e rsge_elglr_setres(rsge_elglr_t* elglr,vec2 res) {
    rsge_unif_render_t ru;
    rsge_error_e err = rsge_unif_render_create(&ru,res);
    if(err != RSGE_ERROR_NONE) return err;
    glViewport(0,0,(GLsizei)res[0],(GLsizei)res[1]);
    glBindBuffer(GL_UNIFORM_BUFFER,elglr->unifBuffs.render.ubo);
    glBufferData(GL_UNIFORM_BUFFER,sizeof(rsge_unif_render_t),&ru,GL_DYNAMIC_DRAW);
    return RSGE_ERROR_NONE;
}

rsge_error_e rsge_elglr_updateSceneUniforms(rsge_elglr_t* elglr,vec4 ambientLightIntensity) {
    rsge_unif_scene_t su;
    rsge_error_e err = rsge_unif_scene_create(&su,ambientLightIntensity);
    if(err != RSGE_ERROR_NONE) return err;
    glBindBuffer(GL_UNIFORM_BUFFER,elglr->unifBuffs.scene.ubo);
    glBufferData(GL_UNIFORM_BUFFER,sizeof(rsge_unif_scene_t),&su,GL_DYNAMIC_DRAW);
    return RSGE_ERROR_NONE;
}

rsge_error_e rsge_elglr_updateCameraUniforms(rsge_elglr_t* elglr,rsge_camera_t* cam) {
    rsge_unif_cam_t cu;
    rsge_error_e err = rsge_unif_cam_create(&cu,cam);
    if(err != RSGE_ERROR_NONE) return err;
    glBindBuffer(GL_UNIFORM_BUFFER,elglr->unifBuffs.camera.ubo);
    glBufferData(GL_UNIFORM_BUFFER,sizeof(rsge_unif_cam_t),&cu,GL_DYNAMIC_DRAW);
    return RSGE_ERROR_NONE;
}

rsge_error_e rsge_elglr_updateObjectUniforms(rsge_elglr_t* elglr,rsge_object_t* obj) {
    rsge_unif_obj_t ou;
    rsge_error_e err = rsge_unif_obj_create(&ou,obj);
    if(err != RSGE_ERROR_NONE) return err;
    glBindBuffer(GL_UNIFORM_BUFFER,elglr->unifBuffs.obj.ubo);
    glBufferData(GL_UNIFORM_BUFFER,sizeof(rsge_unif_obj_t),&ou,GL_DYNAMIC_DRAW);
    return RSGE_ERROR_NONE;
}

rsge_error_e rsge_elglr_updateMaterialUniforms(rsge_elglr_t* elglr,rsge_material_t* mat) {
    rsge_unif_material_t mu;
    rsge_error_e err = rsge_unif_material_create(&mu,mat);
    if(err != RSGE_ERROR_NONE) return err;
    glBindBuffer(GL_UNIFORM_BUFFER,elglr->unifBuffs.mat.ubo);
    glBufferData(GL_UNIFORM_BUFFER,sizeof(rsge_unif_material_t),&mu,GL_DYNAMIC_DRAW);
    return RSGE_ERROR_NONE;
}

rsge_error_e rsge_elglr_updatePointLightUniforms(rsge_elglr_t* elglr,rsge_point_light_t* pl) {
    rsge_unif_point_light_t plu;
    rsge_error_e err = rsge_unif_point_light_create(&plu,pl);
    if(err != RSGE_ERROR_NONE) return err;
    glBindBuffer(GL_UNIFORM_BUFFER,elglr->unifBuffs.pointLight.ubo);
    glBufferData(GL_UNIFORM_BUFFER,sizeof(rsge_unif_point_light_t),&plu,GL_DYNAMIC_DRAW);
    return RSGE_ERROR_NONE;
}

rsge_error_e rsge_elglr_applyState(rsge_elglr_t* elglr,rsge_elglr_state_t* state,rsge_obj_fb_t* framebufferOverride) {
    glUseProgram(state->prg->id);
    if(framebufferOverride == NULL) {
        if(state->fb == NULL) glBindFramebuffer(GL_FRAMEBUFFER,0);
        else glBindFramebuffer(GL_FRAMEBUFFER,state->fb->fbo);
    } else glBindFramebuffer(GL_FRAMEBUFFER,framebufferOverride->fbo);
    glClearDepth(1.0f);
	glClearColor(0,0,0,1);
	glClear((state->clearColor ? GL_COLOR_BUFFER_BIT : 0) | (state->clearDepth ? GL_DEPTH_BUFFER_BIT : 0));
	if(state->useBlending) {
	    glBlendFunc(GL_ONE,GL_ONE);
	    glEnable(GL_BLEND);
	} else glDisable(GL_BLEND);
	if(state->useDepthTest) glEnable(GL_DEPTH_TEST);
	else glDisable(GL_DEPTH_TEST);
    return RSGE_ERROR_NONE;
}

rsge_error_e rsge_elglr_bindTextureArray(rsge_elglr_t* elglr,rsge_obj_texbuff_t* textures,size_t textureCount) {
    for(size_t i = 0;i < textureCount;i++) {
        glActiveTexture(GL_TEXTURE0+i);
        glBindTexture(textures[i].target,textures[i].tex);
    }
    return RSGE_ERROR_NONE;
}

rsge_error_e rsge_elglr_bindTextureList(rsge_elglr_t* elglr,list_t* textures) {
    size_t textureCount = 0;
    list_node_t* node;
    list_iterator_t* it = list_iterator_new(textures,LIST_HEAD);
    if(!it) return RSGE_ERROR_MALLOC;
    while((node = list_iterator_next(it))) textureCount++;
    list_iterator_destroy(it);
    rsge_obj_texbuff_t* textureArr = malloc(sizeof(rsge_obj_texbuff_t)*textureCount);
    if(!textureArr) return RSGE_ERROR_MALLOC;
    it = list_iterator_new(textures,LIST_HEAD);
    if(!it) return RSGE_ERROR_MALLOC;
    size_t textureIndex = 0;
    while((node = list_iterator_next(it))) {
        memcpy(&textureArr[textureIndex++],(rsge_obj_texbuff_t*)node->val,sizeof(rsge_obj_texbuff_t));
    }
    list_iterator_destroy(it);
    return rsge_elglr_bindTextureArray(elglr,textureArr,textureCount);
}

static void rsge_elglr_glFramebufferTexture_impl(GLenum target,GLenum attachment,GLuint texture,GLint level) {
	glFramebufferTexture2D(target,attachment,GL_TEXTURE_2D,texture,level);
}

rsge_error_e rsge_elglr_create(rsge_elglr_t* elglr) {
    memset(elglr,0,sizeof(rsge_elglr_t));
	elglr->texBuffs.bufferedTexsCount = 0;
	int width;
	int height;
	rsge_error_e err = rsge_settings_getint("gfx.res.width",&width);
	if(err != RSGE_ERROR_NONE) return err;
	err = rsge_settings_getint("gfx.res.height",&height);
	if(err != RSGE_ERROR_NONE) return err;
	elglr->resolutionScale = 1.2;
    elglr->resolutions.internal[0] = width*elglr->resolutionScale;
    elglr->resolutions.internal[1] = height*elglr->resolutionScale;
	elglr->resolutions.shadowMap[0] = elglr->resolutions.shadowMap[1] = 1000;
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	if(glFramebufferTexture == NULL) glFramebufferTexture = rsge_elglr_glFramebufferTexture_impl;
	
	err = rsge_elglr_initshaders(elglr);
	if(err != RSGE_ERROR_NONE) return err;
    return RSGE_ERROR_NONE;
}

rsge_error_e rsge_elglr_init() {
	log_debug("OpenGL Version: %s",glGetString(GL_VERSION));
    int glVerMaj;
    glGetIntegerv(GL_MAJOR_VERSION,&glVerMaj);
    int glVerMin;
    glGetIntegerv(GL_MINOR_VERSION,&glVerMin);
    if(glVerMaj < 3 || (glVerMaj == 3 && glVerMin < 2)) {
        log_error("Unsupported OpenGL version, at least OpenGL version 3.2 is required");
        return RSGE_ERROR_OPENGL;
    }
    rsge_error_e err;
	rsge_camera_t* cam;
	err = rsge_camera_getmaincam(&cam);
	if(err != RSGE_ERROR_NONE) return err;
	
	err = rsge_camera_create(cam);
	if(err != RSGE_ERROR_NONE) return err;
    return rsge_elglr_create(&rsge_elglr);
}

rsge_error_e rsge_elglr_deinit() {
    return RSGE_ERROR_NONE;
}
