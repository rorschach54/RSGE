#include <rsge/physics/init.h>
#include <rsge/physics/rigid-body.h>
#include <rsge/physics/world.h>
#include <string.h>

#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_rigid_body_create(rsge_physics_rigid_body_t* rb,rsge_physics_collision_shape_t* shape,int mass) {
    memset(rb,0,sizeof(rsge_physics_rigid_body_t));
    
    btDefaultMotionState* motionstate = new btDefaultMotionState(btTransform(
        btQuaternion(0,0,0,1),
        btVector3(0,0,0)
    ));
    
    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
        mass,
        motionstate,
        shape->btShape,
        btVector3(0,0,0)
    );
    rb->btRB = new btRigidBody(rigidBodyCI);
    return RSGE_ERROR_NONE;
}

#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_rigid_body_destroy(rsge_physics_rigid_body_t* rb) {
    delete rb->btRB;
    memset(rb,0,sizeof(rsge_physics_rigid_body_t));
    return RSGE_ERROR_NONE;
}

#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_rigid_body_setuserpointer(rsge_physics_rigid_body_t* rb,void* userpointer) {
    rb->btRB->setUserPointer(userpointer);
    return RSGE_ERROR_NONE;
}

#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_rigid_body_getuserpointer(rsge_physics_rigid_body_t* rb,void** userpointer) {
    *userpointer = rb->btRB->getUserPointer();
    return RSGE_ERROR_NONE;
}

#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_rigid_body_translate(rsge_physics_rigid_body_t* rb,vec3 pos) {
    rb->btRB->translate(btVector3(pos[0],pos[1],pos[2]));
    return RSGE_ERROR_NONE;
}

#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_rigid_body_getmotionstate(rsge_physics_rigid_body_t* rb,rsge_physics_motion_state_t* motionState) {
    memset(motionState,0,sizeof(rsge_physics_motion_state_t));
    motionState->btMS = rb->btRB->getMotionState();
    return RSGE_ERROR_NONE;
}

#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_rigid_body_setmotionstate(rsge_physics_rigid_body_t* rb,rsge_physics_motion_state_t* motionState) {
    rb->btRB->setMotionState(motionState->btMS);
    return RSGE_ERROR_NONE;
}