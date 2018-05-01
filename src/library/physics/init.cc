#include <rsge/physics/init.h>
#include <btBulletDynamicsCommon.h>
#include <log.h>

btBroadphaseInterface* rsge_physics_bpiface;
btDefaultCollisionConfiguration* rsge_physics_collision_cfg;
btCollisionDispatcher* rsge_physics_collision_dispatcher;
btSequentialImpulseConstraintSolver* rsge_physics_solver;

#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_init() {
    log_debug("Initializing bullet physics");
    rsge_physics_bpiface = new btDbvtBroadphase();
    
    rsge_physics_collision_cfg = new btDefaultCollisionConfiguration();
    rsge_physics_collision_dispatcher = new btCollisionDispatcher(rsge_physics_collision_cfg);
    
    rsge_physics_solver = new btSequentialImpulseConstraintSolver;
    return RSGE_ERROR_NONE;
}

#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_deinit() {
    delete rsge_physics_bpiface;
    delete rsge_physics_collision_cfg;
    delete rsge_physics_collision_dispatcher;
    delete rsge_physics_solver;
    return RSGE_ERROR_NONE;
}