#include <rsge/physics/init.h>
#include <rsge/physics/motion-state.h>
#include <rsge/physics/transform.h>
#include <log.h>
#include <string.h>

#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_motion_state_setworldtransform(rsge_physics_motion_state_t* motionState,rsge_physics_transform_t* transform) {
    btTransform trans;
    trans.setFromOpenGLMatrix(transform->glMatrix);
    motionState->btMS->setWorldTransform(trans);
    return RSGE_ERROR_NONE;
}

#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_motion_state_getworldtransform(rsge_physics_motion_state_t* motionState,rsge_physics_transform_t* transform) {
    memset(transform,0,sizeof(rsge_physics_transform_t));
    
    /* Get the transform */
    btTransform trans;
    motionState->btMS->getWorldTransform(trans);
    
    /* Set basis */
    mat3x3_identity(transform->basis);
    for(int i = 0;i < 3;i++) {
        transform->basis[i][0] = trans.getBasis().getColumn(i).getX();
        transform->basis[i][1] = trans.getBasis().getColumn(i).getY();
        transform->basis[i][2] = trans.getBasis().getColumn(i).getZ();
    }
    
    /* Set origin */
    transform->origin[0] = trans.getOrigin().getX();
    transform->origin[1] = trans.getOrigin().getY();
    transform->origin[2] = trans.getOrigin().getZ();
    
    /* Set rotation */
    quat_identity(transform->rotation);
    vec3 rotation_axis;
    rotation_axis[0] = trans.getRotation().getAxis().getX();
    rotation_axis[1] = trans.getRotation().getAxis().getY();
    rotation_axis[2] = trans.getRotation().getAxis().getZ();
    quat_rotate(transform->rotation,trans.getRotation().getAngle(),rotation_axis);
    
    /* Set gl matrix */
    trans.getOpenGLMatrix(transform->glMatrix);
    return RSGE_ERROR_NONE;
}