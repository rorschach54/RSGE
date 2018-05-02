#include <rsge/physics/collision/box.h>
#include <btBulletDynamicsCommon.h>

#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_box_create(rsge_physics_collision_shape_t* shape,vec3 boxHalfExtents) {
    memset(shape,0,sizeof(rsge_physics_collision_shape_t));
    shape->btShape = new btBoxShape(btVector3(boxHalfExtents[0],boxHalfExtents[1],boxHalfExtents[2]));
    if(shape->btShape == NULL) return RSGE_ERROR_BULLET_PHYSICS;
    return RSGE_ERROR_NONE;
}