#include <rsge/physics/init.h>
#include <rsge/physics/rigid-body.h>
#include <rsge/physics/world.h>
#include <log.h>
#include <string.h>

#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_rigid_body_create(rsge_physics_rigid_body_t* rb,rsge_physics_collision_shape_t* shape,vec3 localInertia,float mass) {
    memset(rb,0,sizeof(rsge_physics_rigid_body_t));
    if(shape->btShape == NULL) {
        log_error("btShape is null");
        return RSGE_ERROR_BULLET_PHYSICS;
    }
    
    btDefaultMotionState* motionstate = new btDefaultMotionState(btTransform(
        btQuaternion(0,0,0,1),
        btVector3(localInertia[0],localInertia[1],localInertia[2])
    ));
    if(motionstate == NULL) return RSGE_ERROR_BULLET_PHYSICS;
    
    btVector3 fallInertia(0,0,0);
    shape->btShape->calculateLocalInertia(mass,fallInertia);
    
    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
        mass,
        motionstate,
        shape->btShape,
        fallInertia
    );
    rb->btRB = new btRigidBody(rigidBodyCI);
    if(rb->btRB == NULL) return RSGE_ERROR_BULLET_PHYSICS;
    return RSGE_ERROR_NONE;
}

#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_rigid_body_destroy(rsge_physics_rigid_body_t* rb) {
    if(rb->btRB == NULL) return RSGE_ERROR_BULLET_PHYSICS;
    delete rb->btRB;
    memset(rb,0,sizeof(rsge_physics_rigid_body_t));
    return RSGE_ERROR_NONE;
}

#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_rigid_body_setuserpointer(rsge_physics_rigid_body_t* rb,void* userpointer) {
    if(rb->btRB == NULL) return RSGE_ERROR_BULLET_PHYSICS;
    rb->btRB->setUserPointer(userpointer);
    return RSGE_ERROR_NONE;
}

#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_rigid_body_getuserpointer(rsge_physics_rigid_body_t* rb,void** userpointer) {
    if(rb->btRB == NULL) return RSGE_ERROR_BULLET_PHYSICS;
    *userpointer = rb->btRB->getUserPointer();
    return RSGE_ERROR_NONE;
}

#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_rigid_body_translate(rsge_physics_rigid_body_t* rb,vec3 pos) {
    if(rb->btRB == NULL) return RSGE_ERROR_BULLET_PHYSICS;
    rb->btRB->translate(btVector3(pos[0],pos[1],pos[2]));
    return RSGE_ERROR_NONE;
}

#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_rigid_body_getmotionstate(rsge_physics_rigid_body_t* rb,rsge_physics_motion_state_t* motionState) {
    if(rb->btRB == NULL) return RSGE_ERROR_BULLET_PHYSICS;
    memset(motionState,0,sizeof(rsge_physics_motion_state_t));
    motionState->btMS = rb->btRB->getMotionState();
    if(motionState->btMS == NULL) return RSGE_ERROR_BULLET_PHYSICS;
    return RSGE_ERROR_NONE;
}

#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_rigid_body_setmotionstate(rsge_physics_rigid_body_t* rb,rsge_physics_motion_state_t* motionState) {
    if(rb->btRB == NULL) return RSGE_ERROR_BULLET_PHYSICS;
    rb->btRB->setMotionState(motionState->btMS);
    return RSGE_ERROR_NONE;
}