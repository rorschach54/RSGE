#include <rsge/physics/init.h>
#include <rsge/physics/world.h>
#include <string.h>

#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_world_create(rsge_physics_world_t* world) {
    memset(world,0,sizeof(rsge_physics_world_t));
    
    world->btWorld = new btDiscreteDynamicsWorld(
        rsge_physics_collision_dispatcher,
        rsge_physics_bpiface,
        rsge_physics_solver,
        rsge_physics_collision_cfg
    );
    world->btWorld->setGravity(btVector3(0,0,0));
    return RSGE_ERROR_NONE;
}

#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_world_destroy(rsge_physics_world_t* world) {
    delete world->btWorld;
    memset(world,0,sizeof(rsge_physics_world_t));
    return RSGE_ERROR_NONE;
}

#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_world_setgravity(rsge_physics_world_t* world,vec3 gravity) {
    world->btWorld->setGravity(btVector3(gravity[0],gravity[1],gravity[2]));
    return RSGE_ERROR_NONE;
}

#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_world_addrb(rsge_physics_world_t* world,rsge_physics_rigid_body_t* rb) {
    world->btWorld->addRigidBody(rb->btRB);
    return RSGE_ERROR_NONE;
}

#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_world_rmrb(rsge_physics_world_t* world,rsge_physics_rigid_body_t* rb) {
    world->btWorld->removeRigidBody(rb->btRB);
    return RSGE_ERROR_NONE;
}

#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_world_stepSimulation(rsge_physics_world_t* world,float timeStep,int maxSubSteps) {
    world->btWorld->stepSimulation(timeStep,maxSubSteps);
    return RSGE_ERROR_NONE;
}