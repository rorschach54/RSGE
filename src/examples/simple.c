#include <rsge/gfx/colors.h>
#include <rsge/gfx/font.h>
#include <rsge/gfx/gl.h>
#include <rsge/gfx/shape.h>
#include <rsge/gfx/surface.h>
#include <rsge/assets/rsge_assets.h>
#include <rsge/assets.h>
#include <rsge/game.h>
#include <math.h>

list_t* objects;
list_t* lights;

rsge_error_e rsge_game_init(rsge_elglr_t* elglr) {
	objects = list_new();
	lights = list_new();
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_game_uninit(rsge_elglr_t* elglr) {
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_game_update(rsge_elglr_t* elglr,float delta,int ticks) {
	rsge_error_e err;
	err = rsge_elglr_setres(elglr,elglr->resolutions.internal);
	if(err != RSGE_ERROR_NONE) return err;
	err = rsge_elglr_updateSceneUniforms(elglr,(vec4){ 0.2f, 0.2f, 0.2f, 1.0f });
	if(err != RSGE_ERROR_NONE) return err;
	err = rsge_elglr_updateCameraUniforms(elglr,NULL);
	if(err != RSGE_ERROR_NONE) return err;
	glBindVertexArray(elglr->vertBuffs.primary.vao);
	
	err = rsge_elglr_applyState(elglr,&elglr->states.buildGBuffs,NULL);
	if(err != RSGE_ERROR_NONE) return err;
	
	int vertOffset = 0;
    list_node_t* node_obj;
    list_iterator_t* it_obj = list_iterator_new(objects,LIST_HEAD);
    if(!it_obj) return RSGE_ERROR_MALLOC;
    while((node_obj = list_iterator_next(it_obj))) {
    	rsge_object_t* obj = (rsge_object_t*)node_obj->val;
    	err = rsge_elglr_updateObjectUniforms(elglr,obj);
    	if(err != RSGE_ERROR_NONE) return err;
    	
    	list_node_t* node_mesh;
    	list_iterator_t* it_mesh = list_iterator_new(obj->meshes,LIST_HEAD);
    	if(!it_mesh) return RSGE_ERROR_MALLOC;
    	while((node_mesh = list_iterator_next(it_mesh))) {
    		rsge_mesh_t* mesh = (rsge_mesh_t*)node_mesh->val;
    		
    		err = rsge_elglr_updateMaterialUniforms(elglr,&mesh->material);
    		if(err != RSGE_ERROR_NONE) return err;
    		
    		err = rsge_elglr_bindTextureArray(elglr,(rsge_obj_texbuff_t[4]){
    			elglr->texBuffs.bufferedTexs[mesh->material.diffuseMapID],
    			elglr->texBuffs.bufferedTexs[mesh->material.specularMapID],
    			elglr->texBuffs.bufferedTexs[mesh->material.bumpMapID],
    			elglr->texBuffs.bufferedTexs[mesh->material.emissivityMapID]
    		},4);
    		if(err != RSGE_ERROR_NONE) return err;
    		glDrawArrays(GL_TRIANGLES,vertOffset,mesh->triangles->len*3);
    		vertOffset += mesh->triangles->len*3;
    	}
    	list_iterator_destroy(it_mesh);
    }
    list_iterator_destroy(it_obj);
    
	err = rsge_elglr_applyState(elglr,&elglr->states.texToScreen,&elglr->frameBuffers.lightingPass);
	if(err != RSGE_ERROR_NONE) return err;
	err = rsge_elglr_bindTextureArray(elglr,(rsge_obj_texbuff_t[1]){
		elglr->texBuffs.firstPassColorBuffers[3]
	},1);
	if(err != RSGE_ERROR_NONE) return err;
	glDrawArrays(GL_TRIANGLE_STRIP,0,4);
	
	err = rsge_elglr_bindTextureArray(elglr,(rsge_obj_texbuff_t[5]){
		elglr->texBuffs.firstPassColorBuffers[0],
		elglr->texBuffs.firstPassColorBuffers[1],
		elglr->texBuffs.firstPassColorBuffers[2],
		elglr->texBuffs.firstPassDepthBuffer,
		elglr->texBuffs.depth
	},5);
	if(err != RSGE_ERROR_NONE) return err;
	
    list_node_t* node_light;
    list_iterator_t* it_light = list_iterator_new(lights,LIST_HEAD);
    if(!it_light) return RSGE_ERROR_MALLOC;
    while((node_light = list_iterator_next(it_light))) {
    	rsge_point_light_t* pointLight = (rsge_point_light_t*)node_light->val;
    	
    	err = rsge_elglr_updatePointLightUniforms(elglr,pointLight);
    	if(err != RSGE_ERROR_NONE) return err;
    	err = rsge_elglr_applyState(elglr,&elglr->states.depthPass,NULL);
		if(err != RSGE_ERROR_NONE) return err;
		err = rsge_elglr_setres(elglr,elglr->resolutions.shadowMap);
		if(err != RSGE_ERROR_NONE) return err;
		
		int vertOffset = 0;
		
	    list_node_t* node_obj;
	    list_iterator_t* it_obj = list_iterator_new(objects,LIST_HEAD);
	    if(!it_obj) return RSGE_ERROR_MALLOC;
	    while((node_obj = list_iterator_next(it_obj))) {
	    	rsge_object_t* obj = (rsge_object_t*)node_obj->val;
	    	err = rsge_elglr_updateObjectUniforms(elglr,obj);
	    	if(err != RSGE_ERROR_NONE) return err;
	    	
	    	list_node_t* node_mesh;
	    	list_iterator_t* it_mesh = list_iterator_new(obj->meshes,LIST_HEAD);
	    	if(!it_mesh) return RSGE_ERROR_MALLOC;
	    	while((node_mesh = list_iterator_next(it_mesh))) {
	    		rsge_mesh_t* mesh = (rsge_mesh_t*)node_mesh->val;
	    		
	    		glDrawArrays(GL_TRIANGLES,vertOffset,mesh->triangles->len*3);
	    		vertOffset += mesh->triangles->len*3;
	    	}
	    	list_iterator_destroy(it_mesh);
	    }
	    list_iterator_destroy(it_obj);
	    
    	err = rsge_elglr_applyState(elglr,&elglr->states.lightingPass,NULL);
		if(err != RSGE_ERROR_NONE) return err;
		err = rsge_elglr_setres(elglr,elglr->resolutions.internal);
		if(err != RSGE_ERROR_NONE) return err;
		
		glDrawArrays(GL_TRIANGLE_STRIP,0,4);
    }
    list_iterator_destroy(it_light);
    
	err = rsge_elglr_applyState(elglr,&elglr->states.texToScreen,NULL);
	if(err != RSGE_ERROR_NONE) return err;
	err = rsge_elglr_bindTextureArray(elglr,(rsge_obj_texbuff_t[1]){
		elglr->texBuffs.lightingPassColor
	},1);
	if(err != RSGE_ERROR_NONE) return err;
	err = rsge_elglr_setres(elglr,elglr->resolutions.output);
	if(err != RSGE_ERROR_NONE) return err;
    glDrawArrays(GL_TRIANGLE_STRIP,0,4);
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_game_getinfo(rsge_game_t* gameinfo) {
	gameinfo->name = "RSGE Example - Simple";
	gameinfo->id = "rsge.example.simple";
	return RSGE_ERROR_NONE;
}