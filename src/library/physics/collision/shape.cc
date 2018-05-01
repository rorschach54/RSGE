#include <rsge/physics/collision/shape.h>
#include <rsge/physics/init.h>
#include <string.h>

#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_shape_destroy(rsge_physics_collision_shape_t* shape) {
    delete shape->btShape;
    memset(shape,0,sizeof(rsge_physics_collision_shape_t));
    return RSGE_ERROR_NONE;
}