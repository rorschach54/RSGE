#include <rsge/physics/collision/box.h>
#include <btBulletDynamicsCommon.h>

#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_box_create(rsge_physics_collision_shape_t* shape,vec3 pos) {
    memset(shape,0,sizeof(rsge_physics_collision_shape_t));
    shape->btShape = new btBoxShape(btVector3(pos[0],pos[1],pos[2]));
    return RSGE_ERROR_NONE;
}