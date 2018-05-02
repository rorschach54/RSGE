#include <rsge/physics/init.h>
#include <rsge/physics/raycasting.h>
#include <rsge/physics/world.h>
#include <string.h>

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
        ray->collisionObject.btObject = ray->RayCallback.m_collisionObject;
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