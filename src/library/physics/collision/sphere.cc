#include <rsge/physics/collision/sphere.h>
#include <btBulletDynamicsCommon.h>

#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_sphere_create(rsge_physics_collision_shape_t* shape,int radius) {
    memset(shape,0,sizeof(rsge_physics_collision_shape_t));
    shape->btShape = new btSphereShape(radius);
    return RSGE_ERROR_NONE;
}