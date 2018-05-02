#include <rsge/physics/init.h>
#include <rsge/physics/transform.h>
#include <string.h>

#ifdef __cplusplus
extern "C"
#endif
rsge_error_e rsge_physics_transform_getorigin(rsge_physics_transform_t* transform,vec3* pos) {
    btVector3 origin = transform->btTF->getOrigin();
    *pos[0] = origin.getX();
    *pos[1] = origin.getY();
    *pos[2] = origin.getZ();
    return RSGE_ERROR_NONE;
}