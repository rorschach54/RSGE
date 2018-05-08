#include <rsge/gfx/uniforms/camera.h>
#include <rsge/physics/init.h>
#include <rsge/physics/raycasting.h>
#include <rsge/physics/world.h>
#include <rsge/input.h>
#include <rsge/settings.h>
#include <string.h>

#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_raycast_createFromMouse(rsge_physics_raycast_t* ray,rsge_physics_world_t* world) {
    vec4 lRayStart;
    vec4 lRayEnd;
    double mouseX;
    double mouseY;
    rsge_error_e err = rsge_input_getmousepos(&mouseX,&mouseY);
    if(err != RSGE_ERROR_NONE) return err;
    int screenWidth;
    err = rsge_settings_getint((char*)"gfx.res.width",&screenWidth);
    if(err != RSGE_ERROR_NONE) return err;
    int screenHeight;
    err = rsge_settings_getint((char*)"gfx.res.height",&screenHeight);
    if(err != RSGE_ERROR_NONE) return err;
    lRayStart[0] = ((float)mouseX/(float)screenWidth-0.5f)*2.0f;
    lRayStart[1] = ((float)mouseY/(float)screenHeight-0.5f)*2.0f;
    lRayStart[2] = -1.0f;
    lRayStart[3] = 1.0f;
    lRayEnd[0] = ((float)mouseX/(float)screenWidth-0.5f)*2.0f;
    lRayEnd[1] = ((float)mouseY/(float)screenHeight-0.5f)*2.0f;
    lRayEnd[2] = 0.0f;
    lRayEnd[3] = 1.0f;
    rsge_unif_cam_t unif;
    err = rsge_unif_cam_create(&unif,NULL);
    if(err != RSGE_ERROR_NONE) return err;
    vec4 lRayStart_camera;
    mat4x4_mul_vec4(lRayStart_camera,unif.inverseProjectionMatrix,lRayStart);
    vec4 lRayStart_world;
    mat4x4_mul_vec4(lRayStart_world,unif.inverseViewMatrix,lRayStart_camera);
    vec4 lRayEnd_camera;
    mat4x4_mul_vec4(lRayEnd_camera,unif.inverseProjectionMatrix,lRayEnd);
    vec4 lRayEnd_world;
    mat4x4_mul_vec4(lRayEnd_world,unif.inverseViewMatrix,lRayEnd_camera);
    vec3 lRayDir_world;
    lRayDir_world[0] = lRayEnd_world[0]-lRayStart_world[0];
    lRayDir_world[1] = lRayEnd_world[1]-lRayStart_world[1];
    lRayDir_world[2] = lRayEnd_world[2]-lRayStart_world[2];
    vec3_norm(lRayDir_world,lRayDir_world);
    return rsge_physics_raycast_create(ray,world,lRayStart_world,lRayDir_world);
}

#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_raycast_create(rsge_physics_raycast_t* ray,rsge_physics_world_t* world,vec3 outOrigin,vec3 outDir) {
    memset(ray,0,sizeof(rsge_physics_raycast_t));
    vec3_scale(outDir,outDir,1000.0f);
    vec3 out_end;
    vec3_add(out_end,outOrigin,outDir);
    btCollisionWorld::ClosestRayResultCallback RayCallback(
	    btVector3(outOrigin[0],outOrigin[1],outOrigin[2]), 
	    btVector3(out_end[0],out_end[1],out_end[2])
    );
    ray->RayCallback = RayCallback;
    world->btWorld->rayTest(
	    btVector3(outOrigin[0],outOrigin[1],outOrigin[2]), 
	    btVector3(out_end[0],out_end[1],out_end[2]),
	    ray->RayCallback
    );
    
    if(ray->RayCallback.hasHit()) {
        ray->collisionObject.btObject = const_cast<btCollisionObject*>(ray->RayCallback.m_collisionObject);
    }
    return RSGE_ERROR_NONE;
}

#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_raycast_destroy(rsge_physics_raycast_t* ray) {
    memset(ray,0,sizeof(rsge_physics_raycast_t));
    return RSGE_ERROR_NONE;
}

#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_raycast_hashit(rsge_physics_raycast_t* ray,int* hit) {
    *hit = ray->RayCallback.hasHit();
    return RSGE_ERROR_NONE;
}