#include <rsge/gfx/colors.h>
#include <rsge/gfx/font.h>
#include <rsge/gfx/gl.h>
#include <rsge/gfx/model.h>
#include <rsge/gfx/surface.h>
#include <rsge/physics/collision/box.h>
#include <rsge/physics/rigid-body.h>
#include <rsge/physics/world.h>
#include <rsge/game.h>
#include <math.h>
#include <log.h>

rsge_shape_t tri;
rsge_physics_world_t world;
rsge_physics_collision_shape_t shape;
rsge_physics_rigid_body_t rb;

rsge_error_e rsge_game_init() {
	rsge_error_e err;
	err = rsge_physics_world_create(&world);
	if(err != RSGE_ERROR_NONE) return err;
	
	vec3 gravity;
	gravity[0] = 0.0f;
	gravity[1] = -0.4f;
	gravity[2] = 0.0f;
	err = rsge_physics_world_setgravity(&world,gravity);
	if(err != RSGE_ERROR_NONE) return err;
	
	vec3 boxHalfExtents;
	boxHalfExtents[0] = 1.0f;
	boxHalfExtents[1] = 1.0f;
	boxHalfExtents[2] = 1.0f;
	err = rsge_physics_box_create(&shape,boxHalfExtents);
	if(err != RSGE_ERROR_NONE) return err;
	
	vec3 localInertia;
	localInertia[0] = 0.0f;
	localInertia[1] = 5.0f;
	localInertia[2] = 0.0f;
	err = rsge_physics_rigid_body_create(&rb,&shape,localInertia,2.0f);
	if(err != RSGE_ERROR_NONE) return err;
	
	err = rsge_physics_world_addrb(&world,&rb);
	if(err != RSGE_ERROR_NONE) return err;
	
	err = rsge_model_fromFile(&tri,"rsge@models/simple.xml");
	if(err != RSGE_ERROR_NONE) return err;

	tri.pos[0] = 0.0f;
	tri.pos[1] = 0.0f;
	tri.pos[2] = 0.0f;
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_game_uninit() {
	rsge_shape_destroy(&tri);
	rsge_physics_rigid_body_destroy(&rb);
	rsge_physics_shape_destroy(&shape);
	rsge_physics_world_destroy(&world);
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_game_update(double time,int fps) {
	rsge_error_e err;
    err = rsge_physics_world_stepSimulation(&world,60.0f,10);
    if(err != RSGE_ERROR_NONE) return err;
    
    rsge_physics_motion_state_t motionState;
    err = rsge_physics_rigid_body_getmotionstate(&rb,&motionState);
    if(err != RSGE_ERROR_NONE) return err;
    
    rsge_physics_transform_t worldTransform;
    err = rsge_physics_motion_state_getworldtransform(&motionState,&worldTransform);
    if(err != RSGE_ERROR_NONE) return err;
    
    tri.pos[0] = worldTransform.origin[0];
    tri.pos[1] = worldTransform.origin[1];
    tri.pos[2] = worldTransform.origin[2];
	err = rsge_shape_render(&tri);
    if(err != RSGE_ERROR_NONE) return err;
	return RSGE_ERROR_NONE;
}

rsge_error_e rsge_game_getinfo(rsge_game_t* gameinfo) {
	gameinfo->name = "RSGE Example - Physics";
	gameinfo->id = "rsge.example.physics";
	return RSGE_ERROR_NONE;
}