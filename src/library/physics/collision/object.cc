#include <rsge/physics/collision/object.h>
#include <rsge/physics/init.h>
#include <string.h>

#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_collision_object_getcollisionshape(rsge_physics_collision_object_t* object,rsge_physics_collision_shape_t* shape) {
    memset(shape,0,sizeof(rsge_physics_collision_shape_t));
    shape->btShape = object->btObject->getCollisionShape();
    return RSGE_ERROR_NONE;
}