#include <rsge/gfx/animation.h>

rsge_error_e rsge_animation_createSurface(rsge_animation_t* animation,rsge_surface_t* surface,rsge_animation_keyframe_t* keyframes,int keyframeCount) {
    memset(animation,0,sizeof(rsge_animation_t));
    
    animation->objectType = RSGE_ANIMATION_OBJECT_TYPE_SURFACE;
    animation->object.surface = surface;
    
    animation->keyframes = keyframes;
    animation->keyframeCount = keyframeCount;
    
    animation->keyframeCurrent = 0;
    return RSGE_ERROR_NONE;
}

rsge_error_e rsge_animation_createShape(rsge_animation_t* animation,rsge_shape_t* shape,rsge_animation_keyframe_t* keyframes,int keyframeCount) {
    memset(animation,0,sizeof(rsge_animation_t));
    
    animation->objectType = RSGE_ANIMATION_OBJECT_TYPE_SHAPE;
    animation->object.shape = shape;
    
    animation->keyframes = keyframes;
    animation->keyframeCount = keyframeCount;
    
    animation->keyframeCurrent = 0;
    return RSGE_ERROR_NONE;
}