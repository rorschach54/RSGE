#include <rsge/physics/init.h>
#include <rsge/physics/motion-state.h>
#include <string.h>

#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_motion_state_setworldtransform(rsge_physics_motion_state_t* motionState,rsge_physics_transform_t* transform) {
    motionState->btMS->setWorldTransform(transform->btTF);
    return RSGE_ERROR_NONE;
}

#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_motion_state_getworldtransform(rsge_physics_motion_state_t* motionState,rsge_physics_transform_t* transform) {
    memset(transform,0,sizeof(rsge_physics_transform_t));
    motionState->btMS->getWorldTransform(transform->btTF);
    return RSGE_ERROR_NONE;
}